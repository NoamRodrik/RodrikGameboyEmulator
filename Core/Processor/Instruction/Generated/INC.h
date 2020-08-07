/**
 * @file        INC.h
 * @author      Noam Rodrik
 * @description Implementation to the INC instruction(s).
 */
#ifndef __LR35902_INC_H__
#define __LR35902_INC_H__

#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>
#include <Core/API/Registers/RegisterPair.h>
#include <Tools/Tools.h>

namespace Core
{
static constexpr data_t INCREMENT_VALUE = 1;

// INC o_reg
auto INC_REG = [](data_t& o_reg)
{
	F.MutateByCondition(Tools::HalfCarryOnAddition(o_reg, INCREMENT_VALUE), Flag::HALF_CARRY);
	o_reg += INCREMENT_VALUE;
	F.MutateByCondition(o_reg == 0, Flag::ZERO);
	F.Clear(Flag::SUB);

	return true;
};

// INC o_reg
auto INC_LONG_REG = [](auto& o_reg)
{
	o_reg += INCREMENT_VALUE;
	return true;
};

// INC (o_reg_addr)
auto INC_REG_ADDR = [](auto& o_reg_addr)
{
	data_t memory_at_address{ DataAt(o_reg_addr) };
	INC_REG(memory_at_address);
	memory.Write(memory_at_address, o_reg_addr);
	return true;
};

// 0x03 INC BC
// - - - -
auto INC_0x03 = []()
{
	return INC_LONG_REG(BC);
};

// 0x04 INC B
// Z 0 H -
auto INC_0x04 = []()
{
	return INC_REG(B);
};

// 0x0C INC C
// Z 0 H -
auto INC_0x0C = []()
{
	return INC_REG(C);
};

// 0x13 INC DE
// - - - -
auto INC_0x13 = []()
{
	return INC_LONG_REG(DE);
};

// 0x14 INC D
// Z 0 H -
auto INC_0x14 = []()
{
	return INC_REG(D);
};

// 0x1C INC E
// Z 0 H -
auto INC_0x1C = []()
{
	return INC_REG(E);
};

// 0x23 INC HL
// - - - -
auto INC_0x23 = []()
{
	return INC_LONG_REG(HL);
};

// 0x24 INC H
// Z 0 H -
auto INC_0x24 = []()
{
	return INC_REG(H);
};

// 0x2C INC L
// Z 0 H -
auto INC_0x2C = []()
{
	return INC_REG(L);
};

// 0x33 INC SP
// - - - -
auto INC_0x33 = []()
{
	return INC_LONG_REG(SP);
};

// 0x34 INC (HL)
// Z 0 H -
auto INC_0x34 = []()
{
	return INC_REG_ADDR(HL_const);
};

// 0x3C INC A
// Z 0 H -
auto INC_0x3C = []()
{
	return INC_REG(A);
};
} // Core

#endif // __LR35902_INC_H__