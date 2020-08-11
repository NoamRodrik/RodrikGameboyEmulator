/**
 * @file        RR.h
 * @author      Noam Rodrik
 * @description Implementation to the RR instruction(s).
 */
#ifndef __LR35902_RR_H__
#define __LR35902_RR_H__

#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>

namespace Core
{
// RR reg
// Z 0 0 C
auto RR = [](auto& reg)
{
	const bool WAS_CARRY_FLAG_SET{F.IsSet(Flag::CARRY)};
	F.MutateByCondition(Tools::IsBitSet(reg, 0), Flag::CARRY);

	// Rotate right reg.
	// The shifted bit goes into the carry flag, the carry flag goes into reg.
	reg >>= 1;
	Tools::MutateBitByCondition(WAS_CARRY_FLAG_SET, reg, 7);

	F.MutateByCondition(reg == 0, Flag::ZERO);
	F.Clear(Flag::SUB);
	F.Clear(Flag::HALF_CARRY);

	return true;
};

// 0x18 RR B
// Z 0 0 C
auto RR_0x18 = []()
{
	return RR(B);
};

// 0x19 RR C
// Z 0 0 C
auto RR_0x19 = []()
{
	return RR(C);
};

// 0x1A RR D
// Z 0 0 C
auto RR_0x1A = []()
{
	return RR(D);
};

// 0x1B RR E
// Z 0 0 C
auto RR_0x1B = []()
{
	return RR(E);
};

// 0x1C RR H
// Z 0 0 C
auto RR_0x1C = []()
{
	return RR(H);
};

// 0x1D RR L
// Z 0 0 C
auto RR_0x1D = []()
{
	return RR(L);
};

// 0x1E RR (HL)
// Z 0 0 C
auto RR_0x1E = []()
{
	return RUN_COMMAND_ON_ADDRESS(HL_const, RR);
};

// 0x1F RR A
// Z 0 0 C
auto RR_0x1F = []()
{
	return RR(A);
};
} // Core

#endif // __LR35902_RR_H__