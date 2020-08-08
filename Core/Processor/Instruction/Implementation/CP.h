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
auto CP_WITH_A = [](const auto& reg)
{
	const int32_t CP_RESULT{static_cast<int32_t>(A_const) - static_cast<int32_t>(reg)};
	
	F.MutateByCondition((CP_RESULT & 0x00FF) == 0, Flag::ZERO);
	F.Set(Flag::SUB);
	F.MutateByCondition(Tools::HalfCarryOnSubtraction(A_const, reg), Flag::HALF_CARRY);
	F.MutateByCondition(CP_RESULT < 0, Flag::CARRY);

	return true;
};

// 0xB8 CP B
// Z 1 H C
auto CP_0xB8 = []()
{
	return CP_WITH_A(B_const);
};

// 0xB9 CP C
// Z 1 H C
auto CP_0xB9 = []()
{
	return CP_WITH_A(C_const);
};

// 0xBA CP D
// Z 1 H C
auto CP_0xBA = []()
{
	return CP_WITH_A(D_const);
};

// 0xBB CP E
// Z 1 H C
auto CP_0xBB = []()
{
	return CP_WITH_A(E_const);
};

// 0xBC CP H
// Z 1 H C
auto CP_0xBC = []()
{
	return CP_WITH_A(H_const);
};

// 0xBD CP L
// Z 1 H C
auto CP_0xBD = []()
{
	return CP_WITH_A(L_const);
};

// 0xBE CP (HL)
// Z 1 H C
auto CP_0xBE = []()
{
	return CP_WITH_A(READ_DATA_AT(HL_const));
};

// 0xBF CP A
// Z 1 H C
auto CP_0xBF = []()
{
	return CP_WITH_A(A_const);
};

// 0xFE CP d8
// Z 1 H C
auto CP_0xFE = []()
{
	return CP_WITH_A(D8());
};
} // Core

#endif // __LR35902_CP_H__