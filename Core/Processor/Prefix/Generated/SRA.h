/**
 * @file        SRA.h
 * @author      Noam Rodrik
 * @description Implementation to the SRA instruction(s).
 */
#ifndef __LR35902_SRA_H__
#define __LR35902_SRA_H__

#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>

namespace Core
{
// SRA reg
// Z 0 0 0
auto SRA_REG = [](auto& reg)
{
	F.MutateByCondition(reg == 0, Flag::ZERO);
	F.Clear(Flag::SUB);
	F.Clear(Flag::HALF_CARRY);
	F.Clear(Flag::CARRY);
	reg >>= 1;
	reg |= static_cast<data_t>(~(-1 >> 1));
	return true;
};

// 0x28 SRA B
// Z 0 0 0
auto SRA_0x28 = []()
{
	return SRA_REG(B);
};

// 0x29 SRA C
// Z 0 0 0
auto SRA_0x29 = []()
{
	return SRA_REG(C);
};

// 0x2A SRA D
// Z 0 0 0
auto SRA_0x2A = []()
{
	return SRA_REG(D);
};

// 0x2B SRA E
// Z 0 0 0
auto SRA_0x2B = []()
{
	return SRA_REG(E);
};

// 0x2C SRA H
// Z 0 0 0
auto SRA_0x2C = []()
{
	return SRA_REG(H);
};

// 0x2D SRA L
// Z 0 0 0
auto SRA_0x2D = []()
{
	return SRA_REG(L);
};

// 0x2E SRA (HL)
// Z 0 0 0
auto SRA_0x2E = []()
{
	return RunCommandAtAddress(HL_const, SRA_REG);
};

// 0x2F SRA A
// Z 0 0 0
auto SRA_0x2F = []()
{
	return SRA_REG(A);
};
} // Core

#endif // __LR35902_SRA_H__