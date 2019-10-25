/**
 * @file		Main.cpp
 * @author		Noam Rodrik
 * @description Program entry point
 */
#include <Core/Processor/Instruction/InstructionLookupTable.h>
#include <Core/API/Registers/RegisterPair.h>
#include <Core/Processor/Processor.h>
#include <stdlib.h>

using namespace Core;

static const auto& arithmetic_regs = Processor::GetInstance().GetRegisters().GetLogicRegisters();
static const auto& stack_reg = Processor::GetInstance().GetRegisters().GetStackRegister();
static const auto& ip_reg = Processor::GetInstance().GetRegisters().GetProgramCounter();

void PrintRegs()
{
	printf_s("A: %02X\nB: %02X\nC: %02X\nD: %02X\nE: %02X\nF: %02X\nH: %02X\nL: %02X\nSP: %04X\nIP: %04X\n\n",
		(data_t)arithmetic_regs.a, (data_t)arithmetic_regs.b,
		(data_t)arithmetic_regs.c, (data_t)arithmetic_regs.d,
		(data_t)arithmetic_regs.e, (data_t)arithmetic_regs.f,
		(data_t)arithmetic_regs.h, (data_t)arithmetic_regs.l,
		(address_t)stack_reg, (address_t)ip_reg);

	printf_s("AF: %04X\nBC: %04X\nDE: %04X\nHL: %04X\n\n",
		(address_t)RegisterPair((data_t&)arithmetic_regs.a, (data_t&)arithmetic_regs.f),
		(address_t)RegisterPair((data_t&)arithmetic_regs.b, (data_t&)arithmetic_regs.c),
		(address_t)RegisterPair((data_t&)arithmetic_regs.d, (data_t&)arithmetic_regs.e),
		(address_t)RegisterPair((data_t&)arithmetic_regs.h, (data_t&)arithmetic_regs.l));
}

#include <Core/GameLoader/GameLoader.h>
#include <errno.h>
int main()
{
#ifdef CHECK_REGS
	PrintRegs();
	for (int i = 0; i < 14; ++i)
	{
		INSTRUCTION_LOOKUP_TABLE[0x04].Execute();
	}
	PrintRegs();
	for (int i = 0; i < 2; ++i)
	{
		INSTRUCTION_LOOKUP_TABLE[0x09].Execute();
	}
	PrintRegs();
#else
	GameLoader load_game{"C:\\Users\\User\\source\\repos\\RodrikGameBoyEmulator\\BootROM\\dmg_boot.bin"};
	Message("Calculate game here!");
#endif
	return EXIT_SUCCESS;
}