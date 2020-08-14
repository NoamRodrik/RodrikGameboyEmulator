/**
 * @file        BIT.h
 * @author      Noam Rodrik
 * @description Implementation to the BIT instruction(s).
 */
#ifndef __LR35902_BIT_H__
#define __LR35902_BIT_H__

#include <Core/CPU/Processor.h>
#include <Core/CPU/Instructions/Shortcuts.h>

using namespace API;

namespace Core
{
// BIT index, data
// Z 0 1 -
static constexpr auto BIT = [](auto&& index, const auto& data)
{
	SANITY(index <= 7, "Got an invalid bit index!");
	F.MutateByCondition(!Tools::IsBitSet(data, index), Flag::ZERO);
	F.Clear(Flag::SUB);
	F.Set(Flag::HALF_CARRY);
	return true;
};

// 0x40 BIT 0,B
// Z 0 1 -
static constexpr auto BIT_0x40 = []()
{
	return BIT(0, B_const);
};

// 0x41 BIT 0,C
// Z 0 1 -
static constexpr auto BIT_0x41 = []()
{
	return BIT(0, C_const);
};

// 0x42 BIT 0,D
// Z 0 1 -
static constexpr auto BIT_0x42 = []()
{
	return BIT(0, D_const);
};

// 0x43 BIT 0,E
// Z 0 1 -
static constexpr auto BIT_0x43 = []()
{
	return BIT(0, E_const);
};

// 0x44 BIT 0,H
// Z 0 1 -
static constexpr auto BIT_0x44 = []()
{
	return BIT(0, H_const);
};

// 0x45 BIT 0,L
// Z 0 1 -
static constexpr auto BIT_0x45 = []()
{
	return BIT(0, L_const);
};

// 0x46 BIT 0,(HL)
// Z 0 1 -
static constexpr auto BIT_0x46 = []()
{
	return BIT(0, READ_DATA_AT(HL_const));
};

// 0x47 BIT 0,A
// Z 0 1 -
static constexpr auto BIT_0x47 = []()
{
	return BIT(0, A_const);
};

// 0x48 BIT 1,B
// Z 0 1 -
static constexpr auto BIT_0x48 = []()
{
	return BIT(1, B_const);
};

// 0x49 BIT 1,C
// Z 0 1 -
static constexpr auto BIT_0x49 = []()
{
	return BIT(1, C_const);
};

// 0x4A BIT 1,D
// Z 0 1 -
static constexpr auto BIT_0x4A = []()
{
	return BIT(1, D_const);
};

// 0x4B BIT 1,E
// Z 0 1 -
static constexpr auto BIT_0x4B = []()
{
	return BIT(1, E_const);
};

// 0x4C BIT 1,H
// Z 0 1 -
static constexpr auto BIT_0x4C = []()
{
	return BIT(1, H_const);
};

// 0x4D BIT 1,L
// Z 0 1 -
static constexpr auto BIT_0x4D = []()
{
	return BIT(1, L_const);
};

// 0x4E BIT 1,(HL)
// Z 0 1 -
static constexpr auto BIT_0x4E = []()
{
	return BIT(1, READ_DATA_AT(HL_const));
};

// 0x4F BIT 1,A
// Z 0 1 -
static constexpr auto BIT_0x4F = []()
{
	return BIT(1, A_const);
};

// 0x50 BIT 2,B
// Z 0 1 -
static constexpr auto BIT_0x50 = []()
{
	return BIT(2, B_const);
};

// 0x51 BIT 2,C
// Z 0 1 -
static constexpr auto BIT_0x51 = []()
{
	return BIT(2, C_const);
};

// 0x52 BIT 2,D
// Z 0 1 -
static constexpr auto BIT_0x52 = []()
{
	return BIT(2, D_const);
};

// 0x53 BIT 2,E
// Z 0 1 -
static constexpr auto BIT_0x53 = []()
{
	return BIT(2, E_const);
};

// 0x54 BIT 2,H
// Z 0 1 -
static constexpr auto BIT_0x54 = []()
{
	return BIT(2, H_const);
};

// 0x55 BIT 2,L
// Z 0 1 -
static constexpr auto BIT_0x55 = []()
{
	return BIT(2, L_const);
};

// 0x56 BIT 2,(HL)
// Z 0 1 -
static constexpr auto BIT_0x56 = []()
{
	return BIT(2, READ_DATA_AT(HL_const));
};

// 0x57 BIT 2,A
// Z 0 1 -
static constexpr auto BIT_0x57 = []()
{
	return BIT(2, A_const);
};

// 0x58 BIT 3,B
// Z 0 1 -
static constexpr auto BIT_0x58 = []()
{
	return BIT(3, B_const);
};

// 0x59 BIT 3,C
// Z 0 1 -
static constexpr auto BIT_0x59 = []()
{
	return BIT(3, C_const);
};

// 0x5A BIT 3,D
// Z 0 1 -
static constexpr auto BIT_0x5A = []()
{
	return BIT(3, D_const);
};

// 0x5B BIT 3,E
// Z 0 1 -
static constexpr auto BIT_0x5B = []()
{
	return BIT(3, E_const);
};

// 0x5C BIT 3,H
// Z 0 1 -
static constexpr auto BIT_0x5C = []()
{
	return BIT(3, H_const);
};

// 0x5D BIT 3,L
// Z 0 1 -
static constexpr auto BIT_0x5D = []()
{
	return BIT(3, L_const);
};

// 0x5E BIT 3,(HL)
// Z 0 1 -
static constexpr auto BIT_0x5E = []()
{
	return BIT(3, READ_DATA_AT(HL_const));
};

// 0x5F BIT 3,A
// Z 0 1 -
static constexpr auto BIT_0x5F = []()
{
	return BIT(3, A_const);
};

// 0x60 BIT 4,B
// Z 0 1 -
static constexpr auto BIT_0x60 = []()
{
	return BIT(4, B_const);
};

// 0x61 BIT 4,C
// Z 0 1 -
static constexpr auto BIT_0x61 = []()
{
	return BIT(4, C_const);
};

// 0x62 BIT 4,D
// Z 0 1 -
static constexpr auto BIT_0x62 = []()
{
	return BIT(4, D_const);
};

// 0x63 BIT 4,E
// Z 0 1 -
static constexpr auto BIT_0x63 = []()
{
	return BIT(4, E_const);
};

// 0x64 BIT 4,H
// Z 0 1 -
static constexpr auto BIT_0x64 = []()
{
	return BIT(4, H_const);
};

// 0x65 BIT 4,L
// Z 0 1 -
static constexpr auto BIT_0x65 = []()
{
	return BIT(4, L_const);
};

// 0x66 BIT 4,(HL)
// Z 0 1 -
static constexpr auto BIT_0x66 = []()
{
	return BIT(4, READ_DATA_AT(HL_const));
};

// 0x67 BIT 4,A
// Z 0 1 -
static constexpr auto BIT_0x67 = []()
{
	return BIT(4, A_const);
};

// 0x68 BIT 5,B
// Z 0 1 -
static constexpr auto BIT_0x68 = []()
{
	return BIT(5, B_const);
};

// 0x69 BIT 5,C
// Z 0 1 -
static constexpr auto BIT_0x69 = []()
{
	return BIT(5, C_const);
};

// 0x6A BIT 5,D
// Z 0 1 -
static constexpr auto BIT_0x6A = []()
{
	return BIT(5, D_const);
};

// 0x6B BIT 5,E
// Z 0 1 -
static constexpr auto BIT_0x6B = []()
{
	return BIT(5, E_const);
};

// 0x6C BIT 5,H
// Z 0 1 -
static constexpr auto BIT_0x6C = []()
{
	return BIT(5, H_const);
};

// 0x6D BIT 5,L
// Z 0 1 -
static constexpr auto BIT_0x6D = []()
{
	return BIT(5, L_const);
};

// 0x6E BIT 5,(HL)
// Z 0 1 -
static constexpr auto BIT_0x6E = []()
{
	return BIT(5, READ_DATA_AT(HL_const));
};

// 0x6F BIT 5,A
// Z 0 1 -
static constexpr auto BIT_0x6F = []()
{
	return BIT(5, A_const);
};

// 0x70 BIT 6,B
// Z 0 1 -
static constexpr auto BIT_0x70 = []()
{
	return BIT(6, B_const);
};

// 0x71 BIT 6,C
// Z 0 1 -
static constexpr auto BIT_0x71 = []()
{
	return BIT(6, C_const);
};

// 0x72 BIT 6,D
// Z 0 1 -
static constexpr auto BIT_0x72 = []()
{
	return BIT(6, D_const);
};

// 0x73 BIT 6,E
// Z 0 1 -
static constexpr auto BIT_0x73 = []()
{
	return BIT(6, E_const);
};

// 0x74 BIT 6,H
// Z 0 1 -
static constexpr auto BIT_0x74 = []()
{
	return BIT(6, H_const);
};

// 0x75 BIT 6,L
// Z 0 1 -
static constexpr auto BIT_0x75 = []()
{
	return BIT(6, L_const);
};

// 0x76 BIT 6,(HL)
// Z 0 1 -
static constexpr auto BIT_0x76 = []()
{
	return BIT(6, READ_DATA_AT(HL_const));
};

// 0x77 BIT 6,A
// Z 0 1 -
static constexpr auto BIT_0x77 = []()
{
	return BIT(6, A_const);
};

// 0x78 BIT 7,B
// Z 0 1 -
static constexpr auto BIT_0x78 = []()
{
	return BIT(7, B_const);
};

// 0x79 BIT 7,C
// Z 0 1 -
static constexpr auto BIT_0x79 = []()
{
	return BIT(7, C_const);
};

// 0x7A BIT 7,D
// Z 0 1 -
static constexpr auto BIT_0x7A = []()
{
	return BIT(7, D_const);
};

// 0x7B BIT 7,E
// Z 0 1 -
static constexpr auto BIT_0x7B = []()
{
	return BIT(7, E_const);
};

// 0x7C BIT 7,H
// Z 0 1 -
static constexpr auto BIT_0x7C = []()
{
	return BIT(7, H_const);
};

// 0x7D BIT 7,L
// Z 0 1 -
static constexpr auto BIT_0x7D = []()
{
	return BIT(7, L_const);
};

// 0x7E BIT 7,(HL)
// Z 0 1 -
static constexpr auto BIT_0x7E = []()
{
	return BIT(7, READ_DATA_AT(HL_const));
};

// 0x7F BIT 7,A
// Z 0 1 -
static constexpr auto BIT_0x7F = []()
{
	return BIT(7, A_const);
};
} // Core

#endif // __LR35902_BIT_H__