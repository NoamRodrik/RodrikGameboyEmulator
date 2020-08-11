/**
 * @file        CP.h
 * @author      Noam Rodrik
 * @description Implementation to the CP instruction(s).
 */
#ifndef __LR35902_CP_H__
#define __LR35902_CP_H__

#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>

namespace Core
{
// CP reg
// Z 1 H C
static constexpr auto CP = [](const auto& reg)
{
	F.MutateByCondition((A_const - reg) == 0, Flag::ZERO);
	F.Set(Flag::SUB);
	F.MutateByCondition((A_const & 0x0F) < (reg & 0x0F), Flag::HALF_CARRY);
	F.MutateByCondition((A_const & 0xFF) < (reg & 0xFF), Flag::CARRY);

	return true;
};

// 0xB8 CP B
// Z 1 H C
static constexpr auto CP_0xB8 = []()
{
	return CP(B_const);
};

// 0xB9 CP C
// Z 1 H C
static constexpr auto CP_0xB9 = []()
{
	return CP(C_const);
};

// 0xBA CP D
// Z 1 H C
static constexpr auto CP_0xBA = []()
{
	return CP(D_const);
};

// 0xBB CP E
// Z 1 H C
static constexpr auto CP_0xBB = []()
{
	return CP(E_const);
};

// 0xBC CP H
// Z 1 H C
static constexpr auto CP_0xBC = []()
{
	return CP(H_const);
};

// 0xBD CP L
// Z 1 H C
static constexpr auto CP_0xBD = []()
{
	return CP(L_const);
};

// 0xBE CP (HL)
// Z 1 H C
static constexpr auto CP_0xBE = []()
{
	return CP(READ_DATA_AT(HL_const));
};

// 0xBF CP A
// Z 1 H C
static constexpr auto CP_0xBF = []()
{
	return CP(A_const);
};

// 0xFE CP d8
// Z 1 H C
static constexpr auto CP_0xFE = []()
{
	return CP(D8());
};
} // Core

#endif // __LR35902_CP_H__