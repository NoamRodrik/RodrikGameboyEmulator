/**
 * @file        INC.h
 * @author      Noam Rodrik
 * @description Implementation to the INC instruction(s).
 */
#ifndef __LR35902_INC_H__
#define __LR35902_INC_H__

#include <Core/CPU/Processor.h>
#include <Core/CPU/Instruction/Shortcuts.h>
#include <Core/API/Registers/RegisterPair.h>
#include <Tools/Tools.h>

namespace Core
{
static constexpr data_t INCREMENT_VALUE = 1;

// INC o_reg
static constexpr auto INC = [](data_t& o_reg)
{
	F.MutateByCondition(Tools::IsBitSet(o_reg, 3) && !Tools::IsBitSet((o_reg + INCREMENT_VALUE), 3), Flag::HALF_CARRY);

	o_reg += INCREMENT_VALUE;

	F.MutateByCondition(o_reg == 0, Flag::ZERO);
	F.Clear(Flag::SUB);

	return true;
};

// INC o_reg
static constexpr auto INC_LONG = [](auto& o_reg)
{
	o_reg += INCREMENT_VALUE;
	return true;
};

// 0x03 INC BC
// - - - -
static constexpr auto INC_0x03 = []()
{
	return INC_LONG(BC);
};

// 0x04 INC B
// Z 0 H -
static constexpr auto INC_0x04 = []()
{
	return INC(B);
};

// 0x0C INC C
// Z 0 H -
static constexpr auto INC_0x0C = []()
{
	return INC(C);
};

// 0x13 INC DE
// - - - -
static constexpr auto INC_0x13 = []()
{
	return INC_LONG(DE);
};

// 0x14 INC D
// Z 0 H -
static constexpr auto INC_0x14 = []()
{
	return INC(D);
};

// 0x1C INC E
// Z 0 H -
static constexpr auto INC_0x1C = []()
{
	return INC(E);
};

// 0x23 INC HL
// - - - -
static constexpr auto INC_0x23 = []()
{
	return INC_LONG(HL);
};

// 0x24 INC H
// Z 0 H -
static constexpr auto INC_0x24 = []()
{
	return INC(H);
};

// 0x2C INC L
// Z 0 H -
static constexpr auto INC_0x2C = []()
{
	return INC(L);
};

// 0x33 INC SP
// - - - -
static constexpr auto INC_0x33 = []()
{
	return INC_LONG(SP);
};

// 0x34 INC (HL)
// Z 0 H -
static constexpr auto INC_0x34 = []()
{
	return RUN_COMMAND_ON_ADDRESS(HL_const, INC);
};

// 0x3C INC A
// Z 0 H -
static constexpr auto INC_0x3C = []()
{
	return INC(A);
};
} // Core

#endif // __LR35902_INC_H__