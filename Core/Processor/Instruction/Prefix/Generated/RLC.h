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

	// Fetching the utmost left bit.
	const uint8_t shifted_bit = (reg & 0x80) >> 7;
	F.MutateByCondition(shifted_bit == 0x01, Flag::CARRY);

	// Rotate left reg.
	// Restore shifted_bit's position.
	reg <<= 1;
	reg |= shifted_bit;
	F.MutateByCondition(reg == 0, Flag::ZERO);
	return true;
};

// RLC (reg_addr)
auto RLC_REG_ADDR = [](const auto& reg_addr)
{
	data_t data_at_address{DataAt(reg_addr)};
	RLC_REG(data_at_address);
	memory.Write(data_at_address, reg_addr);
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
	return RLC_REG_ADDR(HL_const);
};

// 0x07 RLC A
// Z 0 0 C
auto RLC_0x07 = []()
{
	return RLC_REG(A);
};
} // Core

#endif // __LR35902_RLC_H__