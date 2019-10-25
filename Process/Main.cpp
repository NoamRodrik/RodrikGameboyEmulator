/**
 * @file		Main.cpp
 * @author		Noam Rodrik
 * @description Program entry point
 */
#include <Core/Processor/Instruction/InstructionLookupTable.h>
#include <Core/API/Registers/RegisterPair.h>
#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>
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
	BC = 0xABCD;
	PrintRegs();
	INSTRUCTION_LOOKUP_TABLE[0xC5].Execute();
	BC = 0;
	PrintRegs();
	INSTRUCTION_LOOKUP_TABLE[0xC1].Execute();
	PrintRegs();
#else
	IP = 0;
	GameLoader load_game{"C:\\Users\\User\\source\\repos\\RodrikGameBoyEmulator\\BootROM\\dmg_boot.bin"};
	while(IP_const < load_game.GetFileSize())
	{
		const Instruction& command_to_execute = INSTRUCTION_LOOKUP_TABLE[DataAt(IP_const)];
		printf("%04X) %s | ", (uint16_t)IP_const, command_to_execute.operation_string.c_str());
		for (auto index = 0; index < command_to_execute.bytes_size; ++index)
		{
			printf("%02X", DataAt(IP_const + index));
		}
		printf("\r\n");
		if (command_to_execute.Execute())
		{
			IP += command_to_execute.bytes_size;
		}
	}
	
	PrintRegs();

	Message("Calculate game here!");
#endif
	return EXIT_SUCCESS;
}