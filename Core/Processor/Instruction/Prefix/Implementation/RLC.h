/**
 * @file        RLC.h
 * @author      Noam Rodrik
 * @description Implementation to the RLC instruction(s).
 */
#ifndef __LR35902_RLC_H__
#define __LR35902_RLC_H__

#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>

namespace Core
{
// RLC reg
// Z 0 0 C
auto RLC_REG = [](auto& reg)
{
	F.Clear(Flag::SUB);
	F.Clear(Flag::HALF_CARRY);
	
	// Bit 7 is also copied to the carry and also to the new bit 0.
	F.MutateByCondition(Tools::IsBitSet(reg, 7), Flag::CARRY);

	// Rotate left reg and set bit 0 to the carry flag.
	reg <<= 1;

	if (F.IsSet(Flag::CARRY))
	{
		Tools::SetBit(reg, 0);
	}
	else
	{
		Tools::ClearBit(reg, 0);
	}

	F.MutateByCondition(reg == 0, Flag::ZERO);
	return true;
};

// 0x00 RLC B
// Z 0 0 C
auto RLC_0x00 = []()
{
	return RLC_REG(B);
};

// 0x01 RLC C
// Z 0 0 C
auto RLC_0x01 = []()
{
	return RLC_REG(C);
};

// 0x02 RLC D
// Z 0 0 C
auto RLC_0x02 = []()
{
	return RLC_REG(D);
};

// 0x03 RLC E
// Z 0 0 C
auto RLC_0x03 = []()
{
	return RLC_REG(E);
};

// 0x04 RLC H
// Z 0 0 C
auto RLC_0x04 = []()
{
	return RLC_REG(H);
};

// 0x05 RLC L
// Z 0 0 C
auto RLC_0x05 = []()
{
	return RLC_REG(L);
};

// 0x06 RLC (HL)
// Z 0 0 C
auto RLC_0x06 = []()
{
	return RUN_COMMAND_ON_ADDRESS(HL_const, RLC_REG);
};

// 0x07 RLC A
// Z 0 0 C
auto RLC_0x07 = []()
{
	return RLC_REG(A);
};
} // Core

#endif // __LR35902_RLC_H__