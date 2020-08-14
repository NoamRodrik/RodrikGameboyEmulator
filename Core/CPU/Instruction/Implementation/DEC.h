/**
 * @file        DEC.h
 * @author      Noam Rodrik
 * @description Implementation to the DEC instruction(s).
 */
#ifndef __LR35902_DEC_H__
#define __LR35902_DEC_H__

#include <API/Registers/RegisterPair.h>
#include <Core/CPU/Processor.h>
#include <Core/CPU/Instruction/Shortcuts.h>
#include <Tools/Tools.h>

using namespace API;

namespace Core
{
static constexpr data_t DECREMENT_VALUE = 1;

// DEC o_reg
static constexpr auto DEC = [](data_t& o_reg)
{
	F.MutateByCondition((o_reg - DECREMENT_VALUE) == 0, Flag::ZERO);
	F.Set(Flag::SUB);
	F.MutateByCondition(((static_cast<const data_t>(o_reg - DECREMENT_VALUE) ^ o_reg ^ DECREMENT_VALUE) & 0x10) == 0x10, Flag::HALF_CARRY);

	o_reg -= DECREMENT_VALUE;

	return true;
};

// DEC o_long_reg
static constexpr auto DEC_LONG = [](auto& o_long_reg)
{
	o_long_reg -= DECREMENT_VALUE;
	return true;
};

// 0x05 DEC B
// Z 1 H -
static constexpr auto DEC_0x05 = []()
{
	return DEC(B);
};

// 0x0B DEC BC
// - - - -
static constexpr auto DEC_0x0B = []()
{
	return DEC_LONG(BC);
};

// 0x0D DEC C
// Z 1 H -
static constexpr auto DEC_0x0D = []()
{
	return DEC(C);
};

// 0x15 DEC D
// Z 1 H -
static constexpr auto DEC_0x15 = []()
{
	return DEC(D);
};

// 0x1B DEC DE
// - - - -
static constexpr auto DEC_0x1B = []()
{
	return DEC_LONG(DE);
};

// 0x1D DEC E
// Z 1 H -
static constexpr auto DEC_0x1D = []()
{
	return DEC(E);
};

// 0x25 DEC H
// Z 1 H -
static constexpr auto DEC_0x25 = []()
{
	return DEC(H);
};

// 0x2B DEC HL
// - - - -
static constexpr auto DEC_0x2B = []()
{
	return DEC_LONG(HL);
};

// 0x2D DEC L
// Z 1 H -
static constexpr auto DEC_0x2D = []()
{
	return DEC(L);
};

// 0x35 DEC (HL)
// Z 1 H -
static constexpr auto DEC_0x35 = []()
{
	return RUN_COMMAND_ON_ADDRESS(HL_const, DEC);
};

// 0x3B DEC SP
// - - - -
static constexpr auto DEC_0x3B = []()
{
	return DEC_LONG(SP);
};

// 0x3D DEC A
// Z 1 H -
static constexpr auto DEC_0x3D = []()
{
	return DEC(A);
};
} // Core

#endif // __LR35902_DEC_H__