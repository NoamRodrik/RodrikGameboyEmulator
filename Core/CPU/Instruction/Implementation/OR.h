/**
 * @file        OR.h
 * @author      Noam Rodrik
 * @description Implementation to the OR instruction(s).
 */
#ifndef __LR35902_OR_H__
#define __LR35902_OR_H__

#include <Core/CPU/Processor.h>
#include <Core/CPU/Instruction/Shortcuts.h>

using namespace API;

namespace Core
{
// OR data
// Z 0 0 0
static constexpr auto OR = [](const auto& data)
{
	A |= data;

	F.MutateByCondition(A == 0, Flag::ZERO);
	F.Clear(Flag::SUB);
	F.Clear(Flag::HALF_CARRY);
	F.Clear(Flag::CARRY);

	return true;
};

// 0xB0 OR B
// Z 0 0 0
static constexpr auto OR_0xB0 = []()
{
	return OR(B_const);
};

// 0xB1 OR C
// Z 0 0 0
static constexpr auto OR_0xB1 = []()
{
	return OR(C_const);
};

// 0xB2 OR D
// Z 0 0 0
static constexpr auto OR_0xB2 = []()
{
	return OR(D_const);
};

// 0xB3 OR E
// Z 0 0 0
static constexpr auto OR_0xB3 = []()
{
	return OR(E_const);
};

// 0xB4 OR H
// Z 0 0 0
static constexpr auto OR_0xB4 = []()
{
	return OR(H_const);
};

// 0xB5 OR L
// Z 0 0 0
static constexpr auto OR_0xB5 = []()
{
	return OR(L_const);
};

// 0xB6 OR (HL)
// Z 0 0 0
static constexpr auto OR_0xB6 = []()
{
	return OR(READ_DATA_AT(HL_const));
};

// 0xB7 OR A
// Z 0 0 0
static constexpr auto OR_0xB7 = []()
{
	return OR(A_const);
};

// 0xF6 OR d8
// Z 0 0 0
static constexpr auto OR_0xF6 = []()
{
	return OR(D8());
};
} // Core

#endif // __LR35902_OR_H__