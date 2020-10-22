/**
 * @file		Processor.cpp
 * @author		Noam Rodrik
 * @description Main logic of processor
 */
#include <Core/CPU/Interrupts/Registers/InterruptEnable.h>
#include <Core/CPU/Interrupts/Registers/InterruptFlag.h>
#include <Core/CPU/Instructions/General/LookupTable.h>
#include <Core/CPU/Instructions/Prefix/LookupTable.h>
#include <Core/CPU/Interrupts/InterruptHandler.h>
#include <Core/CPU/Instructions/Shortcuts.h>
#include <Core/Engine/MainPixelEngine.h>
#include <Core/CPU/Processor.h>
#include <Core/Timers/Timer.h>
#include <Core/Clock/Clock.h>
#include <Core/APU/APU.h>
#include <Tools/Loader.h>
#include <filesystem>
#include <time.h>
#include <cmath>

using namespace API;

namespace Core
{
gsl::not_null<IPPU*> Processor::GetPPU()
{
	if (this->_ppu.get() == nullptr)
	{
		this->_ppu.reset(gsl::not_null<MainPixelEngine*>{new MainPixelEngine{*this}}.get());
	}

	return this->_ppu.get();
}
	
const size_t Processor::Clock()
{
	size_t clock_cycle{0};
	if (!Processor::IsHalted())
	{
		do
		{
			// The CPU will be unhalted on any triggered interrupt
			const auto& command_to_execute = Processor::IsPrefix() ?
							PREFIX_LOOKUP_TABLE[READ_DATA_AT(PC_const)] :
							GENERAL_LOOKUP_TABLE[READ_DATA_AT(PC_const)];

			// If prefix is enabled, we need to disable it.
			Processor::ClearPrefixCommand();

			// If the command wants to forward the PC, we need to add the bytes size.
			if (command_to_execute.Execute())
			{
				// Change the PC!
				PC += command_to_execute.bytes_size;

				if (command_to_execute.extended_cycles_amount != 0)
				{
					// The operation didn't successfully happen.
					clock_cycle += command_to_execute.extended_cycles_amount;
				}
				else
				{
					// This was not a branching command, so it did successfully happen.
					clock_cycle += command_to_execute.cycles_amount;
				}
			}
			else
			{
				// The operation did successfully happen.
				// The compute amount is the amount of cycles the command needs.
				clock_cycle += command_to_execute.cycles_amount;
			}
		} while (Processor::IsPrefix());
	}
	else
	{
		// While halted, the CPU spins on NOP
		clock_cycle += 1;
	}

	// Interrupts check + Adjust (Takes 5 cycles for a process to dispatch)
	clock_cycle += static_cast<size_t>(InterruptHandler::ProcessInterrupts()) * 5;

	// Clock adjust
	clock_cycle *= 4;

	if (!Processor::IsStopped())
	{
		// If it's not stopped, update devices.
		Processor::GetInstance().GetPPU()->Clock(clock_cycle);
		APU::GetInstance().Clock(clock_cycle);
		Timer::Clock(clock_cycle);
	}

	return clock_cycle;
}

void Processor::LoadGame()
{
    // Choose ROMS from GB folder
	size_t index{1};
	for (const auto& file : std::filesystem::directory_iterator("TestROM"))
	{
		LOG("%llu) %s", index++, file.path().string().c_str());
	}

	uint32_t chosen_index{0};
	do
	{
		LOG("Choose a wanted file from the file list.");
		scanf_s("%u", &chosen_index);
	} while (chosen_index >= index || chosen_index < 1);
	index = chosen_index;

	auto directory_iterator = std::filesystem::directory_iterator("TestROM");
	while (index > 1)
	{
		++directory_iterator;
		--index;
	}

	std::shared_ptr<Tools::Loader> game_loader{std::make_shared<Tools::Loader>((*directory_iterator).path().string())};
	this->_bus.SetMemoryBankController(std::static_pointer_cast<API::ILoader>(game_loader));
}
} // Core