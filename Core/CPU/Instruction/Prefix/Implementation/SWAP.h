/**
 * @file        SWAP.h
 * @author      Noam Rodrik
 * @description Implementation to the SWAP instruction(s).
 */
#ifndef __LR35902_SWAP_H__
#define __LR35902_SWAP_H__

#include <Core/CPU/Processor.h>
#include <Core/CPU/Instruction/Shortcuts.h>

namespace Core
{
// SWAP data
// Z 0 0 0
static constexpr auto SWAP = [](data_t& data)
{
	data = ((data & 0x0F) << 4) | ((data & 0xF0) >> 4);

	F.MutateByCondition(data == 0, Flag::ZERO);
	F.Clear(Flag::SUB);
	F.Clear(Flag::HALF_CARRY);
	F.Clear(Flag::CARRY);

	return true;
};

// 0x30 SWAP B
// Z 0 0 0
static constexpr auto SWAP_0x30 = []()
{
	return SWAP(B);
};

// 0x31 SWAP C
// Z 0 0 0
static constexpr auto SWAP_0x31 = []()
{
	return SWAP(C);
};

// 0x32 SWAP D
// Z 0 0 0
static constexpr auto SWAP_0x32 = []()
{
	return SWAP(D);
};

// 0x33 SWAP E
// Z 0 0 0
static constexpr auto SWAP_0x33 = []()
{
	return SWAP(E);
};

// 0x34 SWAP H
// Z 0 0 0
static constexpr auto SWAP_0x34 = []()
{
	return SWAP(H);
};

// 0x35 SWAP L
// Z 0 0 0
static constexpr auto SWAP_0x35 = []()
{
	return SWAP(L);
};

// 0x36 SWAP (HL)
// Z 0 0 0
static constexpr auto SWAP_0x36 = []()
{
	return RUN_COMMAND_ON_ADDRESS(HL_const, SWAP);
};

// 0x37 SWAP A
// Z 0 0 0
static constexpr auto SWAP_0x37 = []()
{
	return SWAP(A);
};
} // Core

#endif // __LR35902_SWAP_H__