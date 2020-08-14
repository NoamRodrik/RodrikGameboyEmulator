/**
 * @file        ADD.h
 * @author      Noam Rodrik
 * @description Implementation to the ADD instruction(s).
 */
#ifndef __LR35902_ADD_H__
#define __LR35902_ADD_H__

#include <API/Registers/RegisterPair.h>
#include <Core/CPU/Processor.h>
#include <Core/CPU/Instructions/Shortcuts.h>
#include <Tools/Tools.h>

using namespace API;

namespace Core
{
// ADD o_reg, data
// Z 0 H C
static constexpr auto ADD = [](auto& o_reg, const data_t& data)
{
	F.Clear(Flag::SUB);
	F.MutateByCondition(((static_cast<data_t>(o_reg + data) ^ o_reg ^ data) & 0x10) == 0x10, Flag::HALF_CARRY);
	F.MutateByCondition(static_cast<data_t>(o_reg + data) < o_reg, Flag::CARRY);
	o_reg += data;

	F.MutateByCondition(o_reg == 0, Flag::ZERO);
	return true;
};

// ADD o_reg, data
// - 0 H C
static constexpr auto ADD_LONG = [](auto& o_reg, const address_t& data)
{
	F.Clear(Flag::SUB);
	F.MutateByCondition((static_cast<const address_t>(o_reg + data) ^ o_reg ^ data) & 0x1000, Flag::HALF_CARRY);
	F.MutateByCondition(static_cast<const address_t>(o_reg + data) < o_reg, Flag::CARRY);

	o_reg += data;

	return true;
};

// 0x09 ADD HL,BC
// - 0 H C
static constexpr auto ADD_0x09 = []()
{
	return ADD_LONG(HL, BC_const);
};

// 0x19 ADD HL,DE
// - 0 H C
static constexpr auto ADD_0x19 = []()
{
	return ADD_LONG(HL, DE_const);
};

// 0x29 ADD HL,HL
// - 0 H C
static constexpr auto ADD_0x29 = []()
{
	return ADD_LONG(HL, HL_const);
};

// 0x39 ADD HL,SP
// - 0 H C
static constexpr auto ADD_0x39 = []()
{
	return ADD_LONG(HL, SP_const);
};

// 0x80 ADD A,B
// Z 0 H C
static constexpr auto ADD_0x80 = []()
{
	return ADD(A, B_const);
};

// 0x81 ADD A,C
// Z 0 H C
static constexpr auto ADD_0x81 = []()
{
	return ADD(A, C_const);
};

// 0x82 ADD A,D
// Z 0 H C
static constexpr auto ADD_0x82 = []()
{
	return ADD(A, D_const);
};

// 0x83 ADD A,E
// Z 0 H C
static constexpr auto ADD_0x83 = []()
{
	return ADD(A, E_const);
};

// 0x84 ADD A,H
// Z 0 H C
static constexpr auto ADD_0x84 = []()
{
	return ADD(A, H_const);
};

// 0x85 ADD A,L
// Z 0 H C
static constexpr auto ADD_0x85 = []()
{
	return ADD(A, L_const);
};

// 0x86 ADD A,(HL)
// Z 0 H C
static constexpr auto ADD_0x86 = []()
{
	return ADD(A, READ_DATA_AT(HL_const));
};

// 0x87 ADD A,A
// Z 0 H C
static constexpr auto ADD_0x87 = []()
{
	return ADD(A, A_const);
};

// 0xC6 ADD A,d8
// Z 0 H C
static constexpr auto ADD_0xC6 = []()
{
	return ADD(A, D8());
};

// 0xE8 ADD SP,r8
// 0 0 H C
static constexpr auto ADD_0xE8 = []()
{
	F.Clear(Flag::ZERO);
	F.Clear(Flag::SUB);
	F.MutateByCondition((static_cast<const address_t>(SP_const + R8()) & 0xF) < (SP_const & 0xF), Flag::HALF_CARRY);
	F.MutateByCondition((static_cast<const address_t>(SP_const + R8()) & 0xFF) < (SP_const & 0xFF), Flag::CARRY);

	SP += R8();
	return true;
};
} // Core

#endif // __LR35902_ADD_H__