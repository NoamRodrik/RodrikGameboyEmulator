/**
 * @file		Processor.cpp
 * @author		Noam Rodrik
 * @description Main logic of processor
 */

#include <Core/Processor/Instruction/InstructionLookupTable.h>
#include <Core\Interrupts\Registers\InterruptEnable.h>
#include <Core\Interrupts\Registers\InterruptFlag.h>
#include <Core/Processor/Prefix/PrefixLookupTable.h>
#include <Core/Processor/Instruction/Shortcuts.h>
#include <Core/Interrupts/InterruptHandler.h>
#include <Core/Processor/Timer/Timer.h>
#include <Core/Processor/Clock/Clock.h>
#include <Core/Processor/Processor.h>
#include <time.h>
#include <cmath>

#if _DEBUG
#include <Windows.h>
#endif

namespace Core
{
#if _DEBUG
void Processor::PrintInstruction(const Instruction& instruction_to_print)
{
	LOG_NO_ENTER("%04X) %-10s | ", static_cast<const address_t>(PC_const), instruction_to_print.operation_string.c_str());

	const auto WRITE_AMOUNT = instruction_to_print.bytes_size - Processor::IsPrefix();
	for (int32_t index = 0; index < WRITE_AMOUNT; ++index)
	{
		LOG_NO_ENTER("%02X", DataAt(PC_const + index));
	}

	// For alignment
	for (int32_t index = 0; index < 3 - WRITE_AMOUNT; ++index)
	{
		LOG_NO_ENTER("  ");
	}
}

void Processor::PrintRegisters()
{
	LOG_NO_ENTER("A: %02X\nB: %02X\nC: %02X\nD: %02X\nE: %02X\nF: %02X\nH: %02X\nL: %02X\nSP: %04X\nPC: %04X\n",
		static_cast<const data_t>(A_const),	 static_cast<const data_t>(B_const),
		static_cast<const data_t>(C_const),	 static_cast<const data_t>(D_const),
		static_cast<const data_t>(E_const),	 static_cast<const data_t>(F_const),
		static_cast<const data_t>(H_const),	 static_cast<const data_t>(L_const),
		static_cast<const address_t>(SP_const), static_cast<const address_t>(PC_const));
}

void Processor::PrintFlags()
{
	static constexpr char ON[] = "1";
	static constexpr char OFF[] = "0";
	LOG_NO_ENTER(" |");
	LOG_NO_ENTER(" Z = ");
	LOG_NO_ENTER(F.IsSet(Flag::ZERO) ? ON: OFF);
	LOG_NO_ENTER(" N = ");
	LOG_NO_ENTER(F.IsSet(Flag::SUB) ? ON : OFF);
	LOG_NO_ENTER(" H = ");
	LOG_NO_ENTER(F.IsSet(Flag::HALF_CARRY) ? ON : OFF);
	LOG_NO_ENTER(" C = ");
	LOG_NO_ENTER(F.IsSet(Flag::CARRY) ? ON : OFF);
}

void Processor::PrintInterruptRegisters()
{
	LOG_NO_ENTER(" |");
	LOG_NO_ENTER(" IF = ");
	LOG_NO_ENTER("%02hhX", static_cast<data_t>(InterruptFlag{}));
	LOG_NO_ENTER(" IE = ");
	LOG_NO_ENTER("%02hhX", static_cast<data_t>(InterruptEnable{}));
}
#endif
	
const size_t Processor::Clock()
{	
	// Interrupts check.
	size_t clock_cycle = InterruptHandler::ProcessInterrupts();

	if (clock_cycle != 0)
	{
#if _DEBUG
		LOG("Interrupt called!");
#endif
		return clock_cycle;
	}

	if (Processor::IsStopped())
	{
		LOG("STOP called!");

		Message("Make this better!");
		// If there's a button press.
		static_cast<void>(getchar());
		static_cast<void>(getchar());
		Processor::ClearStop();
		LOG("STOP cleared!");

		// Requires no clock cycles
		return 0;
	}

	const auto& command_to_execute = Processor::IsPrefix() ?
										PREFIX_LOOKUP_TABLE[DataAt(PC_const)] :
										INSTRUCTION_LOOKUP_TABLE[DataAt(PC_const)];

#if _DEBUG
	Processor::PrintInstruction(command_to_execute);
#ifndef NO_PRINT_FLAGS
	Processor::PrintFlags();
	LOG_NO_ENTER(" | ->");
#endif
#endif

	// If prefix is enabled, we need to disable it.
	PC -= Processor::IsPrefix();
	Processor::ClearPrefixCommand();

	// The compute amount is the amount of cycles the command needs.
	clock_cycle = command_to_execute.cycles_amount;

	// If the command wants to forward the PC, we need to add the bytes size.
	if (command_to_execute.Execute())
	{
		// Change the PC!
		PC += command_to_execute.bytes_size;

		if (command_to_execute.extended_cycles_amount != 0)
		{
			// Fetch the lower between the two, the operation didn't successfully happen.
			clock_cycle = std::min<decltype(command_to_execute.cycles_amount)>(
								   command_to_execute.cycles_amount,
								   command_to_execute.extended_cycles_amount);
		}
	}
	else
	{
		if (command_to_execute.extended_cycles_amount != 0)
		{
			// Fetch the higher between the two, the operation did successfully happen.
			clock_cycle = std::max<decltype(command_to_execute.cycles_amount)>(
				                   command_to_execute.cycles_amount,
								   command_to_execute.extended_cycles_amount);
		}
	}

#if _DEBUG
#ifndef NO_PRINT
	LOG(" | ");
#endif

#ifndef NO_PRINT_FLAGS
	Processor::PrintFlags();
	LOG(" | ");
#endif

#ifndef NO_PRINT_IF_AND_IE
	Processor::PrintInterruptRegisters();
	LOG(" | ");
#endif

#ifndef NO_PRINT_REGISTERS
	Processor::PrintRegisters();
	LOG(" | ");
#endif
#endif

	clock_cycle += Clock::IsTimerDividerElapsed() ? Timer::IncreaseDivider() : 0;
	clock_cycle += Clock::IsTimerCounterElapsed() ? Timer::IncreaseCounter() : 0;

	return clock_cycle;
}
} // Core