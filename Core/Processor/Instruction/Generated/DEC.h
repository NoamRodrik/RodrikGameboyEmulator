/**
 * @file        DEC.h
 * @author      Noam Rodrik
 * @description Implementation to the DEC instruction(s).
 */
#ifndef __LR35902_DEC_H__
#define __LR35902_DEC_H__

#include <Core/API/Registers/RegisterPair.h>
#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>
#include <Tools/Tools.h>

namespace Core
{
// DEC o_reg
auto DEC_REG = [](data_t& o_reg)
{
	F.MutateByCondition(Tools::HalfCarryOnSubtraction(o_reg, 1), Flag::HALF_CARRY);
	F.MutateByCondition(o_reg - 1 == 0, Flag::ZERO);
	F.Set(Flag::SUB);

	o_reg -= 1;
};

// DEC o_long_reg
auto DEC_LONG_REG = [](auto& o_long_reg)
{
	o_long_reg -= 1;
};

// 0x05 DEC B
// Z 1 H -
auto DEC_0x05 = []()
{
	DEC_REG(B);
};

// 0x0B DEC BC
// - - - -
auto DEC_0x0B = []()
{
	DEC_LONG_REG(BC);
};

// 0x0D DEC C
// Z 1 H -
auto DEC_0x0D = []()
{
	DEC_REG(C);
};

// 0x15 DEC D
// Z 1 H -
auto DEC_0x15 = []()
{
	DEC_REG(D);
};

// 0x1B DEC DE
// - - - -
auto DEC_0x1B = []()
{
	DEC_LONG_REG(DE);
};

// 0x1D DEC E
// Z 1 H -
auto DEC_0x1D = []()
{
	DEC_REG(E);
};

// 0x25 DEC H
// Z 1 H -
auto DEC_0x25 = []()
{
	DEC_REG(H);
};

// 0x2B DEC HL
// - - - -
auto DEC_0x2B = []()
{
	DEC_LONG_REG(HL);
};

// 0x2D DEC L
// Z 1 H -
auto DEC_0x2D = []()
{
	DEC_REG(L);
};

// 0x35 DEC (HL)
// Z 1 H -
auto DEC_0x35 = []()
{
	RunCommandAtAddress(HL_const, DEC_REG);
};

// 0x3B DEC SP
// - - - -
auto DEC_0x3B = []()
{
	DEC_LONG_REG(SP);
};

// 0x3D DEC A
// Z 1 H -
auto DEC_0x3D = []()
{
	DEC_REG(A);
};
} // Core

#endif // __LR35902_DEC_H__