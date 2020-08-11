/**
 * @file        RL.h
 * @author      Noam Rodrik
 * @description Implementation to the RL instruction(s).
 */
#ifndef __LR35902_RL_H__
#define __LR35902_RL_H__

#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>

namespace Core
{
// RL reg
// Z 0 0 C
static constexpr auto RL = [](auto& reg)
{
	const bool WAS_CARRY_ON{F.IsSet(Flag::CARRY)};

	// Fetching the utmost left bit.
	F.MutateByCondition(Tools::IsBitSet(reg, 7), Flag::CARRY);

	// Rotate left reg.
	// The shifted bit goes into the carry flag, the carry flag goes into reg.
	reg <<= 1;
	Tools::MutateBitByCondition(WAS_CARRY_ON, reg, 0);

	F.MutateByCondition(reg == 0, Flag::ZERO);
	F.Clear(Flag::SUB);
	F.Clear(Flag::HALF_CARRY);

	return true;
};

// 0x10 RL B
// Z 0 0 C
static constexpr auto RL_0x10 = []()
{
	return RL(B);
};

// 0x11 RL C
// Z 0 0 C
static constexpr auto RL_0x11 = []()
{
	return RL(C);
};

// 0x12 RL D
// Z 0 0 C
static constexpr auto RL_0x12 = []()
{
	return RL(D);
};

// 0x13 RL E
// Z 0 0 C
static constexpr auto RL_0x13 = []()
{
	return RL(E);
};

// 0x14 RL H
// Z 0 0 C
static constexpr auto RL_0x14 = []()
{
	return RL(H);
};

// 0x15 RL L
// Z 0 0 C
static constexpr auto RL_0x15 = []()
{
	return RL(L);
};

// 0x16 RL (HL)
// Z 0 0 C
static constexpr auto RL_0x16 = []()
{
	return RUN_COMMAND_ON_ADDRESS(HL_const, RL);
};

// 0x17 RL A
// Z 0 0 C
static constexpr auto RL_0x17 = []()
{
	return RL(A);
};
} // Core

#endif // __LR35902_RL_H__