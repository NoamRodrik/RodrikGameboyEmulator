/**
 * @file        RRC.h
 * @author      Noam Rodrik
 * @description Implementation to the RRC instruction(s).
 */
#ifndef __LR35902_RRC_H__
#define __LR35902_RRC_H__

#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>

namespace Core
{
// RRC reg
// Z 0 0 C
auto RRC = [](auto& reg)
{
	F.Clear(Flag::SUB);
	F.Clear(Flag::HALF_CARRY);

	const bool WAS_CARRY_FLAG_SET{F.IsSet(Flag::CARRY)};

	// Fetching the utmost left bit.
	F.MutateByCondition(reg & 0x01, Flag::CARRY);

	// Rotate right A.
	// Restore shifted_bit's position.
	reg = (reg >> 1) | (static_cast<const data_t>(WAS_CARRY_FLAG_SET) << 7);
	F.MutateByCondition(reg == 0, Flag::ZERO);

	return true;
};

// 0x08 RRC B
// Z 0 0 C
auto RRC_0x08 = []()
{
	return RRC(B);
};

// 0x09 RRC C
// Z 0 0 C
auto RRC_0x09 = []()
{
	return RRC(C);
};

// 0x0A RRC D
// Z 0 0 C
auto RRC_0x0A = []()
{
	return RRC(D);
};

// 0x0B RRC E
// Z 0 0 C
auto RRC_0x0B = []()
{
	return RRC(E);
};

// 0x0C RRC H
// Z 0 0 C
auto RRC_0x0C = []()
{
	return RRC(H);
};

// 0x0D RRC L
// Z 0 0 C
auto RRC_0x0D = []()
{
	return RRC(L);
};

// 0x0E RRC (HL)
// Z 0 0 C
auto RRC_0x0E = []()
{
	return RUN_COMMAND_ON_ADDRESS(HL_const, RRC);
};

// 0x0F RRC A
// Z 0 0 C
auto RRC_0x0F = []()
{
	return RRC(A);
};
} // Core

#endif // __LR35902_RRC_H__