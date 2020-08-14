/**
 * @file        XOR.h
 * @author      Noam Rodrik
 * @description Implementation to the XOR instruction(s).
 */
#ifndef __LR35902_XOR_H__
#define __LR35902_XOR_H__

#include <Core/CPU/Processor.h>
#include <Core/CPU/Instructions/Shortcuts.h>

using namespace API;

namespace Core
{
// XOR data
// Z 0 0 0
static constexpr auto XOR = [](const auto data)
{
	A ^= data;

	F.MutateByCondition(A == 0, Flag::ZERO);
	F.Clear(Flag::SUB);
	F.Clear(Flag::HALF_CARRY);
	F.Clear(Flag::CARRY);

	return true;
};

// 0xA8 XOR B
// Z 0 0 0
static constexpr auto XOR_0xA8 = []()
{
	return XOR(B_const);
};

// 0xA9 XOR C
// Z 0 0 0
static constexpr auto XOR_0xA9 = []()
{
	return XOR(C_const);
};

// 0xAA XOR D
// Z 0 0 0
static constexpr auto XOR_0xAA = []()
{
	return XOR(D_const);
};

// 0xAB XOR E
// Z 0 0 0
static constexpr auto XOR_0xAB = []()
{
	return XOR(E_const);
};

// 0xAC XOR H
// Z 0 0 0
static constexpr auto XOR_0xAC = []()
{
	return XOR(H_const);
};

// 0xAD XOR L
// Z 0 0 0
static constexpr auto XOR_0xAD = []()
{
	return XOR(L_const);
};

// 0xAE XOR (HL)
// Z 0 0 0
static constexpr auto XOR_0xAE = []()
{
	return XOR(READ_DATA_AT(HL_const));
};

// 0xAF XOR A
// Z 0 0 0
static constexpr auto XOR_0xAF = []()
{
	return XOR(A_const);
};

// 0xEE XOR d8
// Z 0 0 0
static constexpr auto XOR_0xEE = []()
{
	return XOR(D8());
};
} // Core

#endif // __LR35902_XOR_H__