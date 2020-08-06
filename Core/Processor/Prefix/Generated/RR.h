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
auto RR_REG = [](auto& reg)
{
	F.Clear(Flag::SUB);
	F.Clear(Flag::HALF_CARRY);

	// Fetching the utmost left bit.
	uint8_t shifted_bit = reg & 0x01;
	F.MutateByCondition(shifted_bit == 0x01, Flag::CARRY);

	// Rotate right reg.
	// The shifted bit goes into the carry flag, the carry flag goes into reg.
	reg >>= 1;
	reg = reg | (shifted_bit << 7);
	F.MutateByCondition(reg == 0, Flag::ZERO);
	return true;
};

// 0x18 RR B
// Z 0 0 C
auto RR_0x18 = []()
{
	return RR_REG(B);
};

// 0x19 RR C
// Z 0 0 C
auto RR_0x19 = []()
{
	return RR_REG(C);
};

// 0x1A RR D
// Z 0 0 C
auto RR_0x1A = []()
{
	return RR_REG(D);
};

// 0x1B RR E
// Z 0 0 C
auto RR_0x1B = []()
{
	return RR_REG(E);
};

// 0x1C RR H
// Z 0 0 C
auto RR_0x1C = []()
{
	return RR_REG(H);
};

// 0x1D RR L
// Z 0 0 C
auto RR_0x1D = []()
{
	return RR_REG(L);
};

// 0x1E RR (HL)
// Z 0 0 C
auto RR_0x1E = []()
{
	return RunCommandAtAddress(HL_const, RR_REG);
};

// 0x1F RR A
// Z 0 0 C
auto RR_0x1F = []()
{
	return RR_REG(A);
};
} // Core

#endif // __LR35902_RR_H__