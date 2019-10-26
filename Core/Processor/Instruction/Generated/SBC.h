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
auto SBC_DATA = [](const data_t data)
{
	const uint16_t inverted_data = static_cast<uint16_t>(data) ^ 0x00FF;
	const uint16_t data_to_add = inverted_data + static_cast<uint16_t>(F.IsSet(Flag::CARRY));
	const uint16_t reg_to_deduce = A_const;

	F.MutateByCondition(Tools::ZeroOnAddition(data_to_add, reg_to_deduce), Flag::ZERO);
	F.MutateByCondition(Tools::CarryOnAddition(reg_to_deduce, data_to_add), Flag::CARRY);
	F.Set(Flag::SUB);
	F.MutateByCondition(Tools::HalfCarryOnAddition(reg_to_deduce, data_to_add), Flag::HALF_CARRY);

	A = (reg_to_deduce + data_to_add) & 0x00FF;

	return true;
};

// 0x98 SBC A,B
// Z 1 H C
auto SBC_0x98 = []()
{
	return SBC_DATA(B_const);
};

// 0x99 SBC A,C
// Z 1 H C
auto SBC_0x99 = []()
{
	return SBC_DATA(C_const);
};

// 0x9A SBC A,D
// Z 1 H C
auto SBC_0x9A = []()
{
	return SBC_DATA(D_const);
};

// 0x9B SBC A,E
// Z 1 H C
auto SBC_0x9B = []()
{
	return SBC_DATA(E_const);
};

// 0x9C SBC A,H
// Z 1 H C
auto SBC_0x9C = []()
{
	return SBC_DATA(H_const);
};

// 0x9D SBC A,L
// Z 1 H C
auto SBC_0x9D = []()
{
	return SBC_DATA(L_const);
};

// 0x9E SBC A,(HL)
// Z 1 H C
auto SBC_0x9E = []()
{
	return SBC_DATA(DataAt(HL_const));
};

// 0x9F SBC A,A
// Z 1 H C
auto SBC_0x9F = []()
{
	return SBC_DATA(A_const);
};

// 0xDE SBC A,d8
// Z 1 H C
auto SBC_0xDE = []()
{
	return SBC_DATA(D8());
};
} // Core

#endif // __LR35902_SBC_H__