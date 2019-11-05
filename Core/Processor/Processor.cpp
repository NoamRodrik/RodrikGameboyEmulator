/**
 * @file		Processor.cpp
 * @author		Noam Rodrik
 * @description Main logic of processor
 */
#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/InstructionLookupTable.h>
#include <Core/Processor/Prefix/PrefixLookupTable.h>
#include <Core/Processor/Instruction/Shortcuts.h>
#include <Core/Interrupts/InterruptHandler.h>
#include <Core/Processor/Timer/Timer.h>
#include <time.h>

#if _DEBUG
#include <Windows.h>
#endif

namespace Core
{
#if _DEBUG
void Processor::PrintInstruction(const Instruction& instruction_to_print)
{
	LOG_NO_ENTER("%04X) %-10s | ", static_cast<const address_t>(PC_const), instruction_to_print.operation_string.c_str());

	const auto WRITE_AMOUNT = instruction_to_print.bytes_size - Processor::GetInstance().IsPrefix();
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

	LOG("AF: %04X\nBC: %04X\nDE: %04X\nHL: %04X\n",
		(address_t)AF_const,
		(address_t)BC_const,
		(address_t)DE_const,
		(address_t)HL_const);
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
#endif

void Processor::UpdateClockCycles()
{
	Timer::GetInstance().IncrementCycles();
}
	
const size_t Processor::Clock()
{	
	// Interrupts check.
	size_t clock_cycle = InterruptHandler::ProcessInterrupts();

	if (clock_cycle != 0)
	{
#if _DEBUG
		LOG("Interrupt called!");
		return clock_cycle;
#endif
	}

	const auto& command_to_execute = Processor::GetInstance().IsPrefix() ? PREFIX_LOOKUP_TABLE[DataAt(PC_const)] :
																		   INSTRUCTION_LOOKUP_TABLE[DataAt(PC_const)];
#if _DEBUG
	Processor::PrintInstruction(command_to_execute);
	Processor::PrintFlags();
	LOG_NO_ENTER(" | ->");
#endif

	// If prefix is enabled, we need to disable it.
	PC -= Processor::GetInstance().IsPrefix();
	Processor::GetInstance().ClearPrefixCommand();

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
			clock_cycle = min(command_to_execute.cycles_amount,
							  command_to_execute.extended_cycles_amount);
		}
	}
	else
	{
		if (command_to_execute.extended_cycles_amount != 0)
		{
			// Fetch the higher between the two, the operation did successfully happen.
			clock_cycle = max(command_to_execute.cycles_amount,
							  command_to_execute.extended_cycles_amount);
		}
	}
	
#if _DEBUG
	Processor::PrintFlags();
	LOG(" | ");
#endif

	// Increment timer counter.
	Timer::GetInstance().ClearLoading();
	if (Timer::GetInstance().IsTimerOverflowing())
	{
		// Overflow occurred, call interrupt.
		InterruptHandler::IRQ(EInterrupts::TIMER);
		Timer::GetInstance().AssignCounterToModulo();
		Timer::GetInstance().ClearOverflowing();
		Timer::GetInstance().SetLoading();
	}
	
	return clock_cycle;
}
} // Core