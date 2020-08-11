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
// SRA data
// Z 0 0 C
auto SRA = [](data_t& data)
{
	F.Clear(Flag::SUB);
	F.Clear(Flag::HALF_CARRY);
	F.MutateByCondition(Tools::IsBitSet(data, 0), Flag::CARRY);
	data = ((data >> 1) | (data & 0x80));
	F.MutateByCondition(data == 0, Flag::ZERO);
	return true;
};

// 0x28 SRA B
// Z 0 0 0
auto SRA_0x28 = []()
{
	return SRA(B);
};

// 0x29 SRA C
// Z 0 0 0
auto SRA_0x29 = []()
{
	return SRA(C);
};

// 0x2A SRA D
// Z 0 0 0
auto SRA_0x2A = []()
{
	return SRA(D);
};

// 0x2B SRA E
// Z 0 0 0
auto SRA_0x2B = []()
{
	return SRA(E);
};

// 0x2C SRA H
// Z 0 0 0
auto SRA_0x2C = []()
{
	return SRA(H);
};

// 0x2D SRA L
// Z 0 0 0
auto SRA_0x2D = []()
{
	return SRA(L);
};

// 0x2E SRA (HL)
// Z 0 0 0
auto SRA_0x2E = []()
{
	return RUN_COMMAND_ON_ADDRESS(HL_const, SRA);
};

// 0x2F SRA A
// Z 0 0 0
auto SRA_0x2F = []()
{
	return SRA(A);
};
} // Core

#endif // __LR35902_SRA_H__