/**
 * @file        ADD.h
 * @author      Noam Rodrik
 * @description Implementation to the ADD instruction(s).
 */
#ifndef __LR35902_ADD_H__
#define __LR35902_ADD_H__

#include <Core/API/Registers/RegisterPair.h>
#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>
#include <Tools/Tools.h>

namespace Core
{
// ADD io_reg, data
// - 0 H C
auto ADD_DATA_TO_REG = [](auto& io_reg, const auto& data)
{
	F.Clear(Flag::SUB);
	F.MutateByCondition(Tools::HalfCarryOnAddition(io_reg, data), Flag::HALF_CARRY);
	F.MutateByCondition(Tools::CarryOnAddition(io_reg, data), Flag::CARRY);

	io_reg += data;

	return true;
};

// ADD io_reg, data
// Z 0 H C or 0 0 H C
auto ADD_DATA_TO_REG_MUTATE_ZERO_FLAG = [](auto& io_reg, const auto& data)
{
	F.MutateByCondition(Tools::ZeroOnAddition(io_reg, data), Flag::ZERO);
	return ADD_DATA_TO_REG(io_reg, data);
};

// 0x09 ADD HL,BC
// - 0 H C
auto ADD_0x09 = []()
{
	return ADD_DATA_TO_REG(HL, BC_const);
};

// 0x19 ADD HL,DE
// - 0 H C
auto ADD_0x19 = []()
{
	return ADD_DATA_TO_REG(HL, DE_const);
};

// 0x29 ADD HL,HL
// - 0 H C
auto ADD_0x29 = []()
{
	return ADD_DATA_TO_REG(HL, HL_const);
};

// 0x39 ADD HL,SP
// - 0 H C
auto ADD_0x39 = []()
{
	return ADD_DATA_TO_REG(HL, SP_const);
};

// 0x80 ADD A,B
// Z 0 H C
auto ADD_0x80 = []()
{
	return ADD_DATA_TO_REG_MUTATE_ZERO_FLAG(A, B_const);
};

// 0x81 ADD A,C
// Z 0 H C
auto ADD_0x81 = []()
{
	return ADD_DATA_TO_REG_MUTATE_ZERO_FLAG(A, C_const);
};

// 0x82 ADD A,D
// Z 0 H C
auto ADD_0x82 = []()
{
	return ADD_DATA_TO_REG_MUTATE_ZERO_FLAG(A, D_const);
};

// 0x83 ADD A,E
// Z 0 H C
auto ADD_0x83 = []()
{
	return ADD_DATA_TO_REG_MUTATE_ZERO_FLAG(A, E_const);
};

// 0x84 ADD A,H
// Z 0 H C
auto ADD_0x84 = []()
{
	return ADD_DATA_TO_REG_MUTATE_ZERO_FLAG(A, H_const);
};

// 0x85 ADD A,L
// Z 0 H C
auto ADD_0x85 = []()
{
	return ADD_DATA_TO_REG_MUTATE_ZERO_FLAG(A, L_const);
};

// 0x86 ADD A,(HL)
// Z 0 H C
auto ADD_0x86 = []()
{
	return ADD_DATA_TO_REG_MUTATE_ZERO_FLAG(A, READ_DATA_AT(HL_const));
};

// 0x87 ADD A,A
// Z 0 H C
auto ADD_0x87 = []()
{
	return ADD_DATA_TO_REG_MUTATE_ZERO_FLAG(A, A_const);
};

// 0xC6 ADD A,d8
// Z 0 H C
auto ADD_0xC6 = []()
{
	return ADD_DATA_TO_REG_MUTATE_ZERO_FLAG(A, D8());
};

// 0xE8 ADD SP,r8
// 0 0 H C
auto ADD_0xE8 = []()
{
	F.Clear(Flag::ZERO);
	F.Clear(Flag::SUB);
	F.MutateByCondition(((SP_const & 0x000F) + (R8() & 0x0F)) & 0x00F0, Flag::HALF_CARRY);
	F.MutateByCondition(((SP_const & 0x00FF) + (R8() & 0xFF)) & 0x0F00, Flag::CARRY);
	SP += R8();
	return true;
};
} // Core

#endif // __LR35902_ADD_H__