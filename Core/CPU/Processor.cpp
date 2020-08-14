/**
 * @file		Processor.cpp
 * @author		Noam Rodrik
 * @description Main logic of processor
 */

#include <Core/CPU/Interrupts/Registers/InterruptEnable.h>
#include <Core/CPU/Instructions/Prefix/PrefixLookupTable.h>
#include <Core/CPU/Instructions/InstructionLookupTable.h>
#include <Core/CPU/Interrupts/Registers/InterruptFlag.h>
#include <Core/CPU/Interrupts/InterruptHandler.h>
#include <Core/CPU/Instructions/Shortcuts.h>
#include <Core/CPU/Timers/Timer.h>
#include <Core/CPU/Processor.h>
#include <Core/Clock/Clock.h>
#include <time.h>
#include <cmath>

using namespace API;

namespace Core
{
#if _DEBUG
void Processor::PrintInstruction(const Instruction& instruction_to_print)
{
	LOG("%04X %02X %s ", static_cast<const address_t>(PC_const), READ_DATA_AT(PC_const), instruction_to_print.operation_string.c_str());
}

void Processor::PrintRegisters()
{
	LOG_NO_ENTER("A:%02X B:%02X C:%02X D:%02X E:%02X F:%02X H:%02X L:%02X SP:%04X\n",
		static_cast<const data_t>(A_const),	 static_cast<const data_t>(B_const),
		static_cast<const data_t>(C_const),	 static_cast<const data_t>(D_const),
		static_cast<const data_t>(E_const),	 static_cast<const data_t>(F_const),
		static_cast<const data_t>(H_const),	 static_cast<const data_t>(L_const),
		static_cast<const address_t>(SP_const));
}

void Processor::PrintFlags()
{
	static constexpr char ON[] = "1";
	static constexpr char OFF[] = "0";
	LOG_NO_ENTER("Z:");
	LOG_NO_ENTER(F.IsSet(Flag::ZERO) ? ON: OFF);
	LOG_NO_ENTER(" N:");
	LOG_NO_ENTER(F.IsSet(Flag::SUB) ? ON : OFF);
	LOG_NO_ENTER(" H:");
	LOG_NO_ENTER(F.IsSet(Flag::HALF_CARRY) ? ON : OFF);
	LOG_NO_ENTER(" C:");
	LOG("%s", F.IsSet(Flag::CARRY) ? ON : OFF);
}

void Processor::PrintInterruptRegisters()
{
	LOG_NO_ENTER("IF:");
	LOG_NO_ENTER("%02hhX", static_cast<data_t>(InterruptFlag{}));
	LOG_NO_ENTER(" IE:");
	LOG("%02hhX", static_cast<data_t>(InterruptEnable{}));
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

	// While halted, the CPU spins on NOP
	// The CPU will be unhalted on any triggered interrupt
	const auto& command_to_execute = Processor::IsHalted() ? INSTRUCTION_LOOKUP_TABLE[0] :
									    (Processor::IsPrefix() ?
										 PREFIX_LOOKUP_TABLE[READ_DATA_AT(PC_const)] :
										 INSTRUCTION_LOOKUP_TABLE[READ_DATA_AT(PC_const)]);

#if _DEBUG
	Processor::PrintInstruction(command_to_execute);
#endif

	// If prefix is enabled, we need to disable it.
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
			// The operation didn't successfully happen.
			clock_cycle = command_to_execute.extended_cycles_amount;
		}
	}
	else
	{
		if (command_to_execute.extended_cycles_amount != 0)
		{
			// The operation did successfully happen.
			clock_cycle = command_to_execute.cycles_amount;
		}
	}

#if _DEBUG
#ifndef NO_PRINT_FLAGS
	Processor::PrintFlags();
#endif

#ifndef NO_PRINT_IF_AND_IE
	Processor::PrintInterruptRegisters();
#endif

#ifndef NO_PRINT_REGISTERS
	Processor::PrintRegisters();
#endif

#ifndef NO_PRINT
	LOG("");
#endif
#endif

	// Timer additions
	clock_cycle += Timer::Clock();

	return clock_cycle;
}
} // Core