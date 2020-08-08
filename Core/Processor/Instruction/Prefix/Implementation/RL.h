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
auto RL_REG = [](auto& reg)
{
	F.Clear(Flag::SUB);
	F.Clear(Flag::HALF_CARRY);

	// Fetching the utmost left bit.
	const uint8_t shifted_bit = (reg & 0x80) >> 7;
	const uint8_t already_carried_bit = F.IsSet(Flag::CARRY) & 0x01;
	F.MutateByCondition(shifted_bit == 0x01, Flag::CARRY);

	// Rotate left reg.
	// The shifted bit goes into the carry flag, the carry flag goes into reg.
	reg <<= 1;
	reg |= already_carried_bit;

	F.MutateByCondition(reg == 0, Flag::ZERO);
	return true;
};

// 0x10 RL B
// Z 0 0 C
auto RL_0x10 = []()
{
	return RL_REG(B);
};

// 0x11 RL C
// Z 0 0 C
auto RL_0x11 = []()
{
	return RL_REG(C);
};

// 0x12 RL D
// Z 0 0 C
auto RL_0x12 = []()
{
	return RL_REG(D);
};

// 0x13 RL E
// Z 0 0 C
auto RL_0x13 = []()
{
	return RL_REG(E);
};

// 0x14 RL H
// Z 0 0 C
auto RL_0x14 = []()
{
	return RL_REG(H);
};

// 0x15 RL L
// Z 0 0 C
auto RL_0x15 = []()
{
	return RL_REG(L);
};

// 0x16 RL (HL)
// Z 0 0 C
auto RL_0x16 = []()
{
	return RUN_COMMAND_ON_ADDRESS(HL_const, RL_REG);
};

// 0x17 RL A
// Z 0 0 C
auto RL_0x17 = []()
{
	return RL_REG(A);
};
} // Core

#endif // __LR35902_RL_H__