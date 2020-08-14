/**
 * @file        RES.h
 * @author      Noam Rodrik
 * @description Implementation to the RES instruction(s).
 */
#ifndef __LR35902_RES_H__
#define __LR35902_RES_H__

#include <Core/CPU/Processor.h>
#include <Core/CPU/Instruction/Shortcuts.h>

using namespace API;

namespace Core
{
// RES bit_index, reg
static constexpr auto RES = [](const uint8_t bit_index, auto& reg)
{
	SANITY(bit_index <= 7, "Got an invalid bit index!");
	Tools::ClearBit(reg, bit_index);
	return true;
};

// 0x80 RES 0,B
// - - - -
static constexpr auto RES_0x80 = []()
{
	return RES(0, B);
};

// 0x81 RES 0,C
// - - - -
static constexpr auto RES_0x81 = []()
{
	return RES(0, C);
};

// 0x82 RES 0,D
// - - - -
static constexpr auto RES_0x82 = []()
{
	return RES(0, D);
};

// 0x83 RES 0,E
// - - - -
static constexpr auto RES_0x83 = []()
{
	return RES(0, E);
};

// 0x84 RES 0,H
// - - - -
static constexpr auto RES_0x84 = []()
{
	return RES(0, H);
};

// 0x85 RES 0,L
// - - - -
static constexpr auto RES_0x85 = []()
{
	return RES(0, L);
};

// 0x86 RES 0,(HL)
// - - - -
static constexpr auto RES_0x86 = []()
{
	return RUN_COMMAND_ON_ADDRESS(HL_const, [](auto& addr) {return RES(0, addr); }); 
};

// 0x87 RES 0,A
// - - - -
static constexpr auto RES_0x87 = []()
{
	return RES(0, A);
};

// 0x88 RES 1,B
// - - - -
static constexpr auto RES_0x88 = []()
{
	return RES(1, B);
};

// 0x89 RES 1,C
// - - - -
static constexpr auto RES_0x89 = []()
{
	return RES(1, C);
};

// 0x8A RES 1,D
// - - - -
static constexpr auto RES_0x8A = []()
{
	return RES(1, D);
};

// 0x8B RES 1,E
// - - - -
static constexpr auto RES_0x8B = []()
{
	return RES(1, E);
};

// 0x8C RES 1,H
// - - - -
static constexpr auto RES_0x8C = []()
{
	return RES(1, H);
};

// 0x8D RES 1,L
// - - - -
static constexpr auto RES_0x8D = []()
{
	return RES(1, L);
};

// 0x8E RES 1,(HL)
// - - - -
static constexpr auto RES_0x8E = []()
{
	return RUN_COMMAND_ON_ADDRESS(HL_const, [](auto& addr) {return RES(1, addr); });
};

// 0x8F RES 1,A
// - - - -
static constexpr auto RES_0x8F = []()
{
	return RES(1, A);
};

// 0x90 RES 2,B
// - - - -
static constexpr auto RES_0x90 = []()
{
	return RES(2, B);
};

// 0x91 RES 2,C
// - - - -
static constexpr auto RES_0x91 = []()
{
	return RES(2, C);
};

// 0x92 RES 2,D
// - - - -
static constexpr auto RES_0x92 = []()
{
	return RES(2, D);
};

// 0x93 RES 2,E
// - - - -
static constexpr auto RES_0x93 = []()
{
	return RES(2, E);
};

// 0x94 RES 2,H
// - - - -
static constexpr auto RES_0x94 = []()
{
	return RES(2, H);
};

// 0x95 RES 2,L
// - - - -
static constexpr auto RES_0x95 = []()
{
	return RES(2, L);
};

// 0x96 RES 2,(HL)
// - - - -
static constexpr auto RES_0x96 = []()
{
	return RUN_COMMAND_ON_ADDRESS(HL_const, [](auto& addr) {return RES(2, addr); });
};

// 0x97 RES 2,A
// - - - -
static constexpr auto RES_0x97 = []()
{
	return RES(2, A);
};

// 0x98 RES 3,B
// - - - -
static constexpr auto RES_0x98 = []()
{
	return RES(3, B);
};

// 0x99 RES 3,C
// - - - -
static constexpr auto RES_0x99 = []()
{
	return RES(3, C);
};

// 0x9A RES 3,D
// - - - -
static constexpr auto RES_0x9A = []()
{
	return RES(3, D);
};

// 0x9B RES 3,E
// - - - -
static constexpr auto RES_0x9B = []()
{
	return RES(3, E);
};

// 0x9C RES 3,H
// - - - -
static constexpr auto RES_0x9C = []()
{
	return RES(3, H);
};

// 0x9D RES 3,L
// - - - -
static constexpr auto RES_0x9D = []()
{
	return RES(3, L);
};

// 0x9E RES 3,(HL)
// - - - -
static constexpr auto RES_0x9E = []()
{
	return RUN_COMMAND_ON_ADDRESS(HL_const, [](auto& addr) {return RES(3, addr); });
};

// 0x9F RES 3,A
// - - - -
static constexpr auto RES_0x9F = []()
{
	return RES(3, A);
};

// 0xA0 RES 4,B
// - - - -
static constexpr auto RES_0xA0 = []()
{
	return RES(4, B);
};

// 0xA1 RES 4,C
// - - - -
static constexpr auto RES_0xA1 = []()
{
	return RES(4, C);
};

// 0xA2 RES 4,D
// - - - -
static constexpr auto RES_0xA2 = []()
{
	return RES(4, D);
};

// 0xA3 RES 4,E
// - - - -
static constexpr auto RES_0xA3 = []()
{
	return RES(4, E);
};

// 0xA4 RES 4,H
// - - - -
static constexpr auto RES_0xA4 = []()
{
	return RES(4, H);
};

// 0xA5 RES 4,L
// - - - -
static constexpr auto RES_0xA5 = []()
{
	return RES(4, L);
};

// 0xA6 RES 4,(HL)
// - - - -
static constexpr auto RES_0xA6 = []()
{
	return RUN_COMMAND_ON_ADDRESS(HL_const, [](auto& addr) {return RES(4, addr); });
};

// 0xA7 RES 4,A
// - - - -
static constexpr auto RES_0xA7 = []()
{
	return RES(4, A);
};

// 0xA8 RES 5,B
// - - - -
static constexpr auto RES_0xA8 = []()
{
	return RES(5, B);
};

// 0xA9 RES 5,C
// - - - -
static constexpr auto RES_0xA9 = []()
{
	return RES(5, C);
};

// 0xAA RES 5,D
// - - - -
static constexpr auto RES_0xAA = []()
{
	return RES(5, D);
};

// 0xAB RES 5,E
// - - - -
static constexpr auto RES_0xAB = []()
{
	return RES(5, E);
};

// 0xAC RES 5,H
// - - - -
static constexpr auto RES_0xAC = []()
{
	return RES(5, H);
};

// 0xAD RES 5,L
// - - - -
static constexpr auto RES_0xAD = []()
{
	return RES(5, L);
};

// 0xAE RES 5,(HL)
// - - - -
static constexpr auto RES_0xAE = []()
{
	return RUN_COMMAND_ON_ADDRESS(HL_const, [](auto& addr) {return RES(5, addr); });
};

// 0xAF RES 5,A
// - - - -
static constexpr auto RES_0xAF = []()
{
	return RES(5, A);
};

// 0xB0 RES 6,B
// - - - -
static constexpr auto RES_0xB0 = []()
{
	return RES(6, B);
};

// 0xB1 RES 6,C
// - - - -
static constexpr auto RES_0xB1 = []()
{
	return RES(6, C);
};

// 0xB2 RES 6,D
// - - - -
static constexpr auto RES_0xB2 = []()
{
	return RES(6, D);
};

// 0xB3 RES 6,E
// - - - -
static constexpr auto RES_0xB3 = []()
{
	return RES(6, E);
};

// 0xB4 RES 6,H
// - - - -
static constexpr auto RES_0xB4 = []()
{
	return RES(6, H);
};

// 0xB5 RES 6,L
// - - - -
static constexpr auto RES_0xB5 = []()
{
	return RES(6, L);
};

// 0xB6 RES 6,(HL)
// - - - -
static constexpr auto RES_0xB6 = []()
{
	return RUN_COMMAND_ON_ADDRESS(HL_const, [](auto& addr) {return RES(6, addr); });
};

// 0xB7 RES 6,A
// - - - -
static constexpr auto RES_0xB7 = []()
{
	return RES(6, A);
};

// 0xB8 RES 7,B
// - - - -
static constexpr auto RES_0xB8 = []()
{
	return RES(7, B);
};

// 0xB9 RES 7,C
// - - - -
static constexpr auto RES_0xB9 = []()
{
	return RES(7, C);
};

// 0xBA RES 7,D
// - - - -
static constexpr auto RES_0xBA = []()
{
	return RES(7, D);
};

// 0xBB RES 7,E
// - - - -
static constexpr auto RES_0xBB = []()
{
	return RES(7, E);
};

// 0xBC RES 7,H
// - - - -
static constexpr auto RES_0xBC = []()
{
	return RES(7, H);
};

// 0xBD RES 7,L
// - - - -
static constexpr auto RES_0xBD = []()
{
	return RES(7, L);
};

// 0xBE RES 7,(HL)
// - - - -
static constexpr auto RES_0xBE = []()
{
	return RUN_COMMAND_ON_ADDRESS(HL_const, [](auto& addr) {return RES(7, addr); });
};

// 0xBF RES 7,A
// - - - -
static constexpr auto RES_0xBF = []()
{
	return RES(7, A);
};
} // Core

#endif // __LR35902_RES_H__