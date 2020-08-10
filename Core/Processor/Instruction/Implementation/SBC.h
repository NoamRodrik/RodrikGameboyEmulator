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
auto SBC = [](const auto& data)
{
	const int32_t SBC_RESULT {static_cast<int32_t>(A_const & 0xFF) - static_cast<int32_t>(data & 0xFF) - static_cast<int32_t>(F.IsSet(Flag::CARRY))};

	F.MutateByCondition((SBC_RESULT & 0xFF) == 0, Flag::ZERO);
	F.Set(Flag::SUB);
	F.MutateByCondition((((SBC_RESULT & 0xFF) ^ static_cast<int32_t>(data & 0xFF) ^ static_cast<int32_t>(A_const)) & 0x10) == 0x10, Flag::HALF_CARRY);
	F.MutateByCondition(SBC_RESULT < 0, Flag::CARRY);

	A = static_cast<data_t>(SBC_RESULT & 0xFF);

	return true;
};

// 0x98 SBC A,B
// Z 1 H C
auto SBC_0x98 = []()
{
	return SBC(B_const);
};

// 0x99 SBC A,C
// Z 1 H C
auto SBC_0x99 = []()
{
	return SBC(C_const);
};

// 0x9A SBC A,D
// Z 1 H C
auto SBC_0x9A = []()
{
	return SBC(D_const);
};

// 0x9B SBC A,E
// Z 1 H C
auto SBC_0x9B = []()
{
	return SBC(E_const);
};

// 0x9C SBC A,H
// Z 1 H C
auto SBC_0x9C = []()
{
	return SBC(H_const);
};

// 0x9D SBC A,L
// Z 1 H C
auto SBC_0x9D = []()
{
	return SBC(L_const);
};

// 0x9E SBC A,(HL)
// Z 1 H C
auto SBC_0x9E = []()
{
	return SBC(READ_DATA_AT(HL_const));
};

// 0x9F SBC A,A
// Z 1 H C
auto SBC_0x9F = []()
{
	return SBC(A_const);
};

// 0xDE SBC A,d8
// Z 1 H C
auto SBC_0xDE = []()
{
	return SBC(D8());
};
} // Core

#endif // __LR35902_SBC_H__