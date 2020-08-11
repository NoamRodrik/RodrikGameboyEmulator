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
// SRL data
// Z 0 0 C
auto SRL = [](data_t& data)
{
	F.MutateByCondition(Tools::IsBitSet(data, 0), Flag::CARRY);
	data >>= 1;
	Tools::ClearBit(data, 7);

	F.MutateByCondition(data == 0, Flag::ZERO);
	F.Clear(Flag::SUB);
	F.Clear(Flag::HALF_CARRY);
	return true;
};

// 0x38 SRL B
// Z 0 0 C
auto SRL_0x38 = []()
{
	return SRL(B);
};

// 0x39 SRL C
// Z 0 0 C
auto SRL_0x39 = []()
{
	return SRL(C);
};

// 0x3A SRL D
// Z 0 0 C
auto SRL_0x3A = []()
{
	return SRL(D);
};

// 0x3B SRL E
// Z 0 0 C
auto SRL_0x3B = []()
{
	return SRL(E);
};

// 0x3C SRL H
// Z 0 0 C
auto SRL_0x3C = []()
{
	return SRL(H);
};

// 0x3D SRL L
// Z 0 0 C
auto SRL_0x3D = []()
{
	return SRL(L);
};

// 0x3E SRL (HL)
// Z 0 0 C
auto SRL_0x3E = []()
{
	return RUN_COMMAND_ON_ADDRESS(HL_const, SRL);
};

// 0x3F SRL A
// Z 0 0 C
auto SRL_0x3F = []()
{
	return SRL(A);
};
} // Core

#endif // __LR35902_SRL_H__