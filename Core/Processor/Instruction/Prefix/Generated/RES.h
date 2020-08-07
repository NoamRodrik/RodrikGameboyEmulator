/**
 * @file        RES.h
 * @author      Noam Rodrik
 * @description Implementation to the RES instruction(s).
 */
#ifndef __LR35902_RES_H__
#define __LR35902_RES_H__

#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>

namespace Core
{
// RES bit_index, reg
auto RST_BIT_IN_REG = [](const uint8_t bit_index, auto& reg)
{
	SANITY(bit_index <= 7, "Got an invalid bit index!");
	reg &= static_cast<data_t>(~(1 << bit_index));
	return true;
};

// RES bit_index, (reg_addr)
auto RST_BIT_IN_REG_ADDR = [](const uint8_t bit_index, const auto& reg_addr)
{
	data_t data_at_address{DataAt(reg_addr)};
	RST_BIT_IN_REG(bit_index, data_at_address);
	memory.Write(data_at_address, reg_addr);
	return true;
};

// 0x80 RES 0,B
// - - - -
auto RES_0x80 = []()
{
	return RST_BIT_IN_REG(0, B);
};

// 0x81 RES 0,C
// - - - -
auto RES_0x81 = []()
{
	return RST_BIT_IN_REG(0, C);
};

// 0x82 RES 0,D
// - - - -
auto RES_0x82 = []()
{
	return RST_BIT_IN_REG(0, D);
};

// 0x83 RES 0,E
// - - - -
auto RES_0x83 = []()
{
	return RST_BIT_IN_REG(0, E);
};

// 0x84 RES 0,H
// - - - -
auto RES_0x84 = []()
{
	return RST_BIT_IN_REG(0, H);
};

// 0x85 RES 0,L
// - - - -
auto RES_0x85 = []()
{
	return RST_BIT_IN_REG(0, L);
};

// 0x86 RES 0,(HL)
// - - - -
auto RES_0x86 = []()
{
	return RST_BIT_IN_REG_ADDR(0, HL_const);
};

// 0x87 RES 0,A
// - - - -
auto RES_0x87 = []()
{
	return RST_BIT_IN_REG(0, A);
};

// 0x88 RES 1,B
// - - - -
auto RES_0x88 = []()
{
	return RST_BIT_IN_REG(1, B);
};

// 0x89 RES 1,C
// - - - -
auto RES_0x89 = []()
{
	return RST_BIT_IN_REG(1, C);
};

// 0x8A RES 1,D
// - - - -
auto RES_0x8A = []()
{
	return RST_BIT_IN_REG(1, D);
};

// 0x8B RES 1,E
// - - - -
auto RES_0x8B = []()
{
	return RST_BIT_IN_REG(1, E);
};

// 0x8C RES 1,H
// - - - -
auto RES_0x8C = []()
{
	return RST_BIT_IN_REG(1, H);
};

// 0x8D RES 1,L
// - - - -
auto RES_0x8D = []()
{
	return RST_BIT_IN_REG(1, L);
};

// 0x8E RES 1,(HL)
// - - - -
auto RES_0x8E = []()
{
	return RST_BIT_IN_REG_ADDR(1, HL_const);
};

// 0x8F RES 1,A
// - - - -
auto RES_0x8F = []()
{
	return RST_BIT_IN_REG(1, A);
};

// 0x90 RES 2,B
// - - - -
auto RES_0x90 = []()
{
	return RST_BIT_IN_REG(2, B);
};

// 0x91 RES 2,C
// - - - -
auto RES_0x91 = []()
{
	return RST_BIT_IN_REG(2, C);
};

// 0x92 RES 2,D
// - - - -
auto RES_0x92 = []()
{
	return RST_BIT_IN_REG(2, D);
};

// 0x93 RES 2,E
// - - - -
auto RES_0x93 = []()
{
	return RST_BIT_IN_REG(2, E);
};

// 0x94 RES 2,H
// - - - -
auto RES_0x94 = []()
{
	return RST_BIT_IN_REG(2, H);
};

// 0x95 RES 2,L
// - - - -
auto RES_0x95 = []()
{
	return RST_BIT_IN_REG(2, L);
};

// 0x96 RES 2,(HL)
// - - - -
auto RES_0x96 = []()
{
	return RST_BIT_IN_REG_ADDR(2, HL_const);
};

// 0x97 RES 2,A
// - - - -
auto RES_0x97 = []()
{
	return RST_BIT_IN_REG(2, A);
};

// 0x98 RES 3,B
// - - - -
auto RES_0x98 = []()
{
	return RST_BIT_IN_REG(3, B);
};

// 0x99 RES 3,C
// - - - -
auto RES_0x99 = []()
{
	return RST_BIT_IN_REG(3, C);
};

// 0x9A RES 3,D
// - - - -
auto RES_0x9A = []()
{
	return RST_BIT_IN_REG(3, D);
};

// 0x9B RES 3,E
// - - - -
auto RES_0x9B = []()
{
	return RST_BIT_IN_REG(3, E);
};

// 0x9C RES 3,H
// - - - -
auto RES_0x9C = []()
{
	return RST_BIT_IN_REG(3, H);
};

// 0x9D RES 3,L
// - - - -
auto RES_0x9D = []()
{
	return RST_BIT_IN_REG(3, L);
};

// 0x9E RES 3,(HL)
// - - - -
auto RES_0x9E = []()
{
	return RST_BIT_IN_REG_ADDR(3, HL_const);
};

// 0x9F RES 3,A
// - - - -
auto RES_0x9F = []()
{
	return RST_BIT_IN_REG(3, A);
};

// 0xA0 RES 4,B
// - - - -
auto RES_0xA0 = []()
{
	return RST_BIT_IN_REG(4, B);
};

// 0xA1 RES 4,C
// - - - -
auto RES_0xA1 = []()
{
	return RST_BIT_IN_REG(4, C);
};

// 0xA2 RES 4,D
// - - - -
auto RES_0xA2 = []()
{
	return RST_BIT_IN_REG(4, D);
};

// 0xA3 RES 4,E
// - - - -
auto RES_0xA3 = []()
{
	return RST_BIT_IN_REG(4, E);
};

// 0xA4 RES 4,H
// - - - -
auto RES_0xA4 = []()
{
	return RST_BIT_IN_REG(4, H);
};

// 0xA5 RES 4,L
// - - - -
auto RES_0xA5 = []()
{
	return RST_BIT_IN_REG(4, L);
};

// 0xA6 RES 4,(HL)
// - - - -
auto RES_0xA6 = []()
{
	return RST_BIT_IN_REG_ADDR(4, HL_const);
};

// 0xA7 RES 4,A
// - - - -
auto RES_0xA7 = []()
{
	return RST_BIT_IN_REG(4, A);
};

// 0xA8 RES 5,B
// - - - -
auto RES_0xA8 = []()
{
	return RST_BIT_IN_REG(5, B);
};

// 0xA9 RES 5,C
// - - - -
auto RES_0xA9 = []()
{
	return RST_BIT_IN_REG(5, C);
};

// 0xAA RES 5,D
// - - - -
auto RES_0xAA = []()
{
	return RST_BIT_IN_REG(5, D);
};

// 0xAB RES 5,E
// - - - -
auto RES_0xAB = []()
{
	return RST_BIT_IN_REG(5, E);
};

// 0xAC RES 5,H
// - - - -
auto RES_0xAC = []()
{
	return RST_BIT_IN_REG(5, H);
};

// 0xAD RES 5,L
// - - - -
auto RES_0xAD = []()
{
	return RST_BIT_IN_REG(5, L);
};

// 0xAE RES 5,(HL)
// - - - -
auto RES_0xAE = []()
{
	return RST_BIT_IN_REG_ADDR(5, HL_const);
};

// 0xAF RES 5,A
// - - - -
auto RES_0xAF = []()
{
	return RST_BIT_IN_REG(5, A);
};

// 0xB0 RES 6,B
// - - - -
auto RES_0xB0 = []()
{
	return RST_BIT_IN_REG(6, B);
};

// 0xB1 RES 6,C
// - - - -
auto RES_0xB1 = []()
{
	return RST_BIT_IN_REG(6, C);
};

// 0xB2 RES 6,D
// - - - -
auto RES_0xB2 = []()
{
	return RST_BIT_IN_REG(6, D);
};

// 0xB3 RES 6,E
// - - - -
auto RES_0xB3 = []()
{
	return RST_BIT_IN_REG(6, E);
};

// 0xB4 RES 6,H
// - - - -
auto RES_0xB4 = []()
{
	return RST_BIT_IN_REG(6, H);
};

// 0xB5 RES 6,L
// - - - -
auto RES_0xB5 = []()
{
	return RST_BIT_IN_REG(6, L);
};

// 0xB6 RES 6,(HL)
// - - - -
auto RES_0xB6 = []()
{
	return RST_BIT_IN_REG_ADDR(6, HL_const);
};

// 0xB7 RES 6,A
// - - - -
auto RES_0xB7 = []()
{
	return RST_BIT_IN_REG(6, A);
};

// 0xB8 RES 7,B
// - - - -
auto RES_0xB8 = []()
{
	return RST_BIT_IN_REG(7, B);
};

// 0xB9 RES 7,C
// - - - -
auto RES_0xB9 = []()
{
	return RST_BIT_IN_REG(7, C);
};

// 0xBA RES 7,D
// - - - -
auto RES_0xBA = []()
{
	return RST_BIT_IN_REG(7, D);
};

// 0xBB RES 7,E
// - - - -
auto RES_0xBB = []()
{
	return RST_BIT_IN_REG(7, E);
};

// 0xBC RES 7,H
// - - - -
auto RES_0xBC = []()
{
	return RST_BIT_IN_REG(7, H);
};

// 0xBD RES 7,L
// - - - -
auto RES_0xBD = []()
{
	return RST_BIT_IN_REG(7, L);
};

// 0xBE RES 7,(HL)
// - - - -
auto RES_0xBE = []()
{
	return RST_BIT_IN_REG_ADDR(7, HL_const);
};

// 0xBF RES 7,A
// - - - -
auto RES_0xBF = []()
{
	return RST_BIT_IN_REG(7, A);
};
} // Core

#endif // __LR35902_RES_H__