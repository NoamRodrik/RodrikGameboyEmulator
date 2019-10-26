/**
 * @file        SUB.h
 * @author      Noam Rodrik
 * @description Implementation to the SUB instruction(s).
 */
#ifndef __LR35902_SUB_H__
#define __LR35902_SUB_H__

#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>

namespace Core
{
// SUB io_reg, data
// Z 1 H C
auto SUB_DATA_FROM_REG = [](auto& io_reg, const auto& data)
{
	F.MutateByCondition(Tools::ZeroOnSubtraction(io_reg, data), Flag::ZERO);
	F.Set(Flag::SUB);
	F.MutateByCondition(Tools::HalfCarryOnSubtraction(io_reg, data), Flag::HALF_CARRY);
	F.MutateByCondition(Tools::CarryOnSubtraction(io_reg, data), Flag::CARRY);

	io_reg += data;
	return true;
};

// 0x90 SUB B
// Z 1 H C
auto SUB_0x90 = []()
{
	return SUB_DATA_FROM_REG(A, B_const);
};

// 0x91 SUB C
// Z 1 H C
auto SUB_0x91 = []()
{
	return SUB_DATA_FROM_REG(A, C_const);
};

// 0x92 SUB D
// Z 1 H C
auto SUB_0x92 = []()
{
	return SUB_DATA_FROM_REG(A, C_const);
};

// 0x93 SUB E
// Z 1 H C
auto SUB_0x93 = []()
{
	return SUB_DATA_FROM_REG(A, E_const);
};

// 0x94 SUB H
// Z 1 H C
auto SUB_0x94 = []()
{
	return SUB_DATA_FROM_REG(A, H_const);
};

// 0x95 SUB L
// Z 1 H C
auto SUB_0x95 = []()
{
	return SUB_DATA_FROM_REG(A, L_const);
};

// 0x96 SUB (HL)
// Z 1 H C
auto SUB_0x96 = []()
{
	return SUB_DATA_FROM_REG(A, DataAt(HL_const));
};

// 0x97 SUB A
// Z 1 H C
auto SUB_0x97 = []()
{
	return SUB_DATA_FROM_REG(A, A_const);
};

// 0xD6 SUB d8
// Z 1 H C
auto SUB_0xD6 = []()
{
	return SUB_DATA_FROM_REG(A, D8());
};
} // Core

#endif // __LR35902_SUB_H__