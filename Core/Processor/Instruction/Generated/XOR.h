/**
 * @file        XOR.h
 * @author      Noam Rodrik
 * @description Implementation to the XOR instruction(s).
 */
#ifndef __LR35902_XOR_H__
#define __LR35902_XOR_H__

#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>

namespace Core
{
// XOR data
// Z 0 0 0
auto XOR_WITH_A = [](const auto data)
{
	A ^= data;

	F.MutateByCondition(A == 0, Flag::ZERO);
	F.Clear(Flag::SUB);
	F.Clear(Flag::CARRY);
	F.Clear(Flag::HALF_CARRY);
	return true;
};

// 0xA8 XOR B
// Z 0 0 0
auto XOR_0xA8 = []()
{
	return XOR_WITH_A(B_const);
};

// 0xA9 XOR C
// Z 0 0 0
auto XOR_0xA9 = []()
{
	return XOR_WITH_A(C_const);
};

// 0xAA XOR D
// Z 0 0 0
auto XOR_0xAA = []()
{
	return XOR_WITH_A(D_const);
};

// 0xAB XOR E
// Z 0 0 0
auto XOR_0xAB = []()
{
	return XOR_WITH_A(E_const);
};

// 0xAC XOR H
// Z 0 0 0
auto XOR_0xAC = []()
{
	return XOR_WITH_A(H_const);
};

// 0xAD XOR L
// Z 0 0 0
auto XOR_0xAD = []()
{
	return XOR_WITH_A(L_const);
};

// 0xAE XOR (HL)
// Z 0 0 0
auto XOR_0xAE = []()
{
	return XOR_WITH_A(DataAt(HL_const));
};

// 0xAF XOR A
// Z 0 0 0
auto XOR_0xAF = []()
{
	return XOR_WITH_A(A_const);
};

// 0xEE XOR d8
// Z 0 0 0
auto XOR_0xEE = []()
{
	return XOR_WITH_A(DataAt(IP_const + 1));
};
} // Core

#endif // __LR35902_XOR_H__