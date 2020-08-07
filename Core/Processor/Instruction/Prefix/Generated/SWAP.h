/**
 * @file        SWAP.h
 * @author      Noam Rodrik
 * @description Implementation to the SWAP instruction(s).
 */
#ifndef __LR35902_SWAP_H__
#define __LR35902_SWAP_H__

#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>

namespace Core
{
// SWAP reg
// Z 0 0 0
auto SWAP_REG = [](auto& reg)
{
	F.MutateByCondition(reg == 0, Flag::ZERO);
	F.Clear(Flag::SUB);
	F.Clear(Flag::HALF_CARRY);
	F.Clear(Flag::CARRY);

	reg = ((reg & 0x0F) << 4) | ((reg & 0xF0) >> 4);
	return true;
};

// 0x30 SWAP B
// Z 0 0 0
auto SWAP_0x30 = []()
{
	return SWAP_REG(B);
};

// 0x31 SWAP C
// Z 0 0 0
auto SWAP_0x31 = []()
{
	return SWAP_REG(C);
};

// 0x32 SWAP D
// Z 0 0 0
auto SWAP_0x32 = []()
{
	return SWAP_REG(D);
};

// 0x33 SWAP E
// Z 0 0 0
auto SWAP_0x33 = []()
{
	return SWAP_REG(E);
};

// 0x34 SWAP H
// Z 0 0 0
auto SWAP_0x34 = []()
{
	return SWAP_REG(H);
};

// 0x35 SWAP L
// Z 0 0 0
auto SWAP_0x35 = []()
{
	return SWAP_REG(L);
};

// 0x36 SWAP (HL)
// Z 0 0 0
auto SWAP_0x36 = []()
{
	return RunCommandAtAddress(HL_const, SWAP_REG);
};

// 0x37 SWAP A
// Z 0 0 0
auto SWAP_0x37 = []()
{
	return SWAP_REG(A);
};
} // Core

#endif // __LR35902_SWAP_H__