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
	F.MutateByCondition(Tools::IsBitSet(reg, 0), Flag::CARRY);

	// Rotate right the register and set the carry bit in it's place.
	reg >>= 1;
	Tools::MutateBitByCondition(F.IsSet(Flag::CARRY), reg, 7);

	F.MutateByCondition(reg == 0, Flag::ZERO);
	F.Clear(Flag::SUB);
	F.Clear(Flag::HALF_CARRY);

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