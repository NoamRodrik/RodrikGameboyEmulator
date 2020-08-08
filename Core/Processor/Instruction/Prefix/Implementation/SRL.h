/**
 * @file        SRL.h
 * @author      Noam Rodrik
 * @description Implementation to the SRL instruction(s).
 */
#ifndef __LR35902_SRL_H__
#define __LR35902_SRL_H__

#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>

namespace Core
{
// SRL reg
// Z 0 0 C
auto SRL_REG = [](auto& reg)
{
	F.Clear(Flag::SUB);
	F.Clear(Flag::HALF_CARRY);
	F.MutateByCondition(reg & 0x01, Flag::CARRY);
	reg >>= 1;
	F.MutateByCondition(reg == 0, Flag::ZERO);
	return true;
};

// 0x38 SRL B
// Z 0 0 C
auto SRL_0x38 = []()
{
	return SRL_REG(B);
};

// 0x39 SRL C
// Z 0 0 C
auto SRL_0x39 = []()
{
	return SRL_REG(C);
};

// 0x3A SRL D
// Z 0 0 C
auto SRL_0x3A = []()
{
	return SRL_REG(D);
};

// 0x3B SRL E
// Z 0 0 C
auto SRL_0x3B = []()
{
	return SRL_REG(E);
};

// 0x3C SRL H
// Z 0 0 C
auto SRL_0x3C = []()
{
	return SRL_REG(H);
};

// 0x3D SRL L
// Z 0 0 C
auto SRL_0x3D = []()
{
	return SRL_REG(L);
};

// 0x3E SRL (HL)
// Z 0 0 C
auto SRL_0x3E = []()
{
	return RUN_COMMAND_ON_ADDRESS(HL_const, SRL_REG);
};

// 0x3F SRL A
// Z 0 0 C
auto SRL_0x3F = []()
{
	return SRL_REG(A);
};
} // Core

#endif // __LR35902_SRL_H__