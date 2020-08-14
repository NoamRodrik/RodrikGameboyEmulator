/**
 * @file        AND.h
 * @author      Noam Rodrik
 * @description Implementation to the AND instruction(s).
 */
#ifndef __LR35902_AND_H__
#define __LR35902_AND_H__

#include <Core/CPU/Processor.h>
#include <Core/CPU/Instruction/Shortcuts.h>

namespace Core
{
// AND data
// Z 0 1 0
static constexpr auto AND = [](const auto data)
{
	A &= data;

	F.MutateByCondition(A == 0, Flag::ZERO);
	F.Clear(Flag::SUB);
	F.Set(Flag::HALF_CARRY);
	F.Clear(Flag::CARRY);

	return true;
};

// 0xA0 AND B
// Z 0 1 0
static constexpr auto AND_0xA0 = []()
{
	return AND(B_const);
};

// 0xA1 AND C
// Z 0 1 0
static constexpr auto AND_0xA1 = []()
{
	return AND(C_const);
};

// 0xA2 AND D
// Z 0 1 0
static constexpr auto AND_0xA2 = []()
{
	return AND(D_const);
};

// 0xA3 AND E
// Z 0 1 0
static constexpr auto AND_0xA3 = []()
{
	return AND(E_const);
};

// 0xA4 AND H
// Z 0 1 0
static constexpr auto AND_0xA4 = []()
{
	return AND(H_const);
};

// 0xA5 AND L
// Z 0 1 0
static constexpr auto AND_0xA5 = []()
{
	return AND(L_const);
};

// 0xA6 AND (HL)
// Z 0 1 0
static constexpr auto AND_0xA6 = []()
{
	return AND(READ_DATA_AT(HL_const));
};

// 0xA7 AND A
// Z 0 1 0
static constexpr auto AND_0xA7 = []()
{
	return AND(A_const);
};

// 0xE6 AND d8
// Z 0 1 0
static constexpr auto AND_0xE6 = []()
{
	return AND(D8());
};
} // Core

#endif // __LR35902_AND_H__