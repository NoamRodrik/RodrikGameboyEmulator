/**
 * @file        SBC.h
 * @author      Noam Rodrik
 * @description Implementation to the SBC instruction(s).
 */
#ifndef __LR35902_SBC_H__
#define __LR35902_SBC_H__

#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>

namespace Core
{
// SBC A, data
// Z 1 H C
auto SBC_WITH_A = [](const auto& data)
{
	const data_t data_to_subtract = static_cast<data_t>(data) + static_cast<data_t>(F.IsSet(Flag::CARRY));

	F.MutateByCondition(Tools::ZeroOnSubtraction(data_to_subtract, static_cast<data_t>(A_const)), Flag::ZERO);
	F.MutateByCondition(Tools::CarryOnSubtraction(static_cast<data_t>(A_const), data_to_subtract), Flag::CARRY);
	F.Set(Flag::SUB);
	F.MutateByCondition(Tools::HalfCarryOnSubtraction(static_cast<data_t>(A_const), data_to_subtract), Flag::HALF_CARRY);

	A = (static_cast<data_t>(A_const) - data_to_subtract) & 0x00FF;

	return true;
};

// 0x98 SBC A,B
// Z 1 H C
auto SBC_0x98 = []()
{
	return SBC_WITH_A(B_const);
};

// 0x99 SBC A,C
// Z 1 H C
auto SBC_0x99 = []()
{
	return SBC_WITH_A(C_const);
};

// 0x9A SBC A,D
// Z 1 H C
auto SBC_0x9A = []()
{
	return SBC_WITH_A(D_const);
};

// 0x9B SBC A,E
// Z 1 H C
auto SBC_0x9B = []()
{
	return SBC_WITH_A(E_const);
};

// 0x9C SBC A,H
// Z 1 H C
auto SBC_0x9C = []()
{
	return SBC_WITH_A(H_const);
};

// 0x9D SBC A,L
// Z 1 H C
auto SBC_0x9D = []()
{
	return SBC_WITH_A(L_const);
};

// 0x9E SBC A,(HL)
// Z 1 H C
auto SBC_0x9E = []()
{
	return SBC_WITH_A(DataAt(HL_const));
};

// 0x9F SBC A,A
// Z 1 H C
auto SBC_0x9F = []()
{
	return SBC_WITH_A(A_const);
};

// 0xDE SBC A,d8
// Z 1 H C
auto SBC_0xDE = []()
{
	return SBC_WITH_A(FETCH_D8());
};
} // Core

#endif // __LR35902_SBC_H__