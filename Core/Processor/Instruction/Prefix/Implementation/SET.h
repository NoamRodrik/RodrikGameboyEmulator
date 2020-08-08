/**
 * @file        SET.h
 * @author      Noam Rodrik
 * @description Implementation to the SET instruction(s).
 */
#ifndef __LR35902_SET_H__
#define __LR35902_SET_H__

#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>

namespace Core
{
// SET bit_index, reg
auto SET_BIT_IN_REG = [](const uint8_t bit_index, auto& reg)
{
	SANITY(bit_index <= 7, "Got an invalid bit index!");
	reg |= static_cast<data_t>(1 << bit_index);
	return true;
};

// 0xC0 SET 0,B
// - - - -
auto SET_0xC0 = []()
{
	return SET_BIT_IN_REG(0, B);
};

// 0xC1 SET 0,C
// - - - -
auto SET_0xC1 = []()
{
	return SET_BIT_IN_REG(0, C);
};

// 0xC2 SET 0,D
// - - - -
auto SET_0xC2 = []()
{
	return SET_BIT_IN_REG(0, D);
};

// 0xC3 SET 0,E
// - - - -
auto SET_0xC3 = []()
{
	return SET_BIT_IN_REG(0, E);
};

// 0xC4 SET 0,H
// - - - -
auto SET_0xC4 = []()
{
	return SET_BIT_IN_REG(0, H);
};

// 0xC5 SET 0,L
// - - - -
auto SET_0xC5 = []()
{
	return SET_BIT_IN_REG(0, L);
};

// 0xC6 SET 0,(HL)
// - - - -
auto SET_0xC6 = []()
{
	const auto SET_REG_WITH_0 = [](auto& reg) { return SET_BIT_IN_REG(0, reg); };
	return RUN_COMMAND_ON_ADDRESS(HL_const, SET_REG_WITH_0);
};

// 0xC7 SET 0,A
// - - - -
auto SET_0xC7 = []()
{
	return SET_BIT_IN_REG(0, A);
};

// 0xC8 SET 1,B
// - - - -
auto SET_0xC8 = []()
{
	return SET_BIT_IN_REG(1, B);
};

// 0xC9 SET 1,C
// - - - -
auto SET_0xC9 = []()
{
	return SET_BIT_IN_REG(1, C);
};

// 0xCA SET 1,D
// - - - -
auto SET_0xCA = []()
{
	return SET_BIT_IN_REG(1, D);
};

// 0xCB SET 1,E
// - - - -
auto SET_0xCB = []()
{
	return SET_BIT_IN_REG(1, E);
};

// 0xCC SET 1,H
// - - - -
auto SET_0xCC = []()
{
	return SET_BIT_IN_REG(1, H);
};

// 0xCD SET 1,L
// - - - -
auto SET_0xCD = []()
{
	return SET_BIT_IN_REG(1, L);
};

// 0xCE SET 1,(HL)
// - - - -
auto SET_0xCE = []()
{
	const auto SET_REG_WITH_1 = [](auto& reg) { return SET_BIT_IN_REG(1, reg); };
	return RUN_COMMAND_ON_ADDRESS(HL_const, SET_REG_WITH_1);
};

// 0xCF SET 1,A
// - - - -
auto SET_0xCF = []()
{
	return SET_BIT_IN_REG(1, A);
};

// 0xD0 SET 2,B
// - - - -
auto SET_0xD0 = []()
{
	return SET_BIT_IN_REG(2, B);
};

// 0xD1 SET 2,C
// - - - -
auto SET_0xD1 = []()
{
	return SET_BIT_IN_REG(2, C);
};

// 0xD2 SET 2,D
// - - - -
auto SET_0xD2 = []()
{
	return SET_BIT_IN_REG(2, D);
};

// 0xD3 SET 2,E
// - - - -
auto SET_0xD3 = []()
{
	return SET_BIT_IN_REG(2, E);
};

// 0xD4 SET 2,H
// - - - -
auto SET_0xD4 = []()
{
	return SET_BIT_IN_REG(2, H);
};

// 0xD5 SET 2,L
// - - - -
auto SET_0xD5 = []()
{
	return SET_BIT_IN_REG(2, L);
};

// 0xD6 SET 2,(HL)
// - - - -
auto SET_0xD6 = []()
{
	const auto SET_REG_WITH_2 = [](auto& reg) { return SET_BIT_IN_REG(2, reg); };
	return RUN_COMMAND_ON_ADDRESS(HL_const, SET_REG_WITH_2);
};

// 0xD7 SET 2,A
// - - - -
auto SET_0xD7 = []()
{
	return SET_BIT_IN_REG(2, A);
};

// 0xD8 SET 3,B
// - - - -
auto SET_0xD8 = []()
{
	return SET_BIT_IN_REG(3, B);
};

// 0xD9 SET 3,C
// - - - -
auto SET_0xD9 = []()
{
	return SET_BIT_IN_REG(3, C);
};

// 0xDA SET 3,D
// - - - -
auto SET_0xDA = []()
{
	return SET_BIT_IN_REG(3, D);
};

// 0xDB SET 3,E
// - - - -
auto SET_0xDB = []()
{
	return SET_BIT_IN_REG(3, E);
};

// 0xDC SET 3,H
// - - - -
auto SET_0xDC = []()
{
	return SET_BIT_IN_REG(3, H);
};

// 0xDD SET 3,L
// - - - -
auto SET_0xDD = []()
{
	return SET_BIT_IN_REG(3, L);
};

// 0xDE SET 3,(HL)
// - - - -
auto SET_0xDE = []()
{
	const auto SET_REG_WITH_3 = [](auto& reg) { return SET_BIT_IN_REG(3, reg); };
	return RUN_COMMAND_ON_ADDRESS(HL_const, SET_REG_WITH_3);
};

// 0xDF SET 3,A
// - - - -
auto SET_0xDF = []()
{
	return SET_BIT_IN_REG(3, A);
};

// 0xE0 SET 4,B
// - - - -
auto SET_0xE0 = []()
{
	return SET_BIT_IN_REG(4, B);
};

// 0xE1 SET 4,C
// - - - -
auto SET_0xE1 = []()
{
	return SET_BIT_IN_REG(4, C);
};

// 0xE2 SET 4,D
// - - - -
auto SET_0xE2 = []()
{
	return SET_BIT_IN_REG(4, D);
};

// 0xE3 SET 4,E
// - - - -
auto SET_0xE3 = []()
{
	return SET_BIT_IN_REG(4, E);
};

// 0xE4 SET 4,H
// - - - -
auto SET_0xE4 = []()
{
	return SET_BIT_IN_REG(4, H);
};

// 0xE5 SET 4,L
// - - - -
auto SET_0xE5 = []()
{
	return SET_BIT_IN_REG(4, L);
};

// 0xE6 SET 4,(HL)
// - - - -
auto SET_0xE6 = []()
{
	const auto SET_REG_WITH_4 = [](auto& reg) { return SET_BIT_IN_REG(4, reg); };
	return RUN_COMMAND_ON_ADDRESS(HL_const, SET_REG_WITH_4);
};

// 0xE7 SET 4,A
// - - - -
auto SET_0xE7 = []()
{
	return SET_BIT_IN_REG(4, A);
};

// 0xE8 SET 5,B
// - - - -
auto SET_0xE8 = []()
{
	return SET_BIT_IN_REG(5, B);
};

// 0xE9 SET 5,C
// - - - -
auto SET_0xE9 = []()
{
	return SET_BIT_IN_REG(5, C);
};

// 0xEA SET 5,D
// - - - -
auto SET_0xEA = []()
{
	return SET_BIT_IN_REG(5, D);
};

// 0xEB SET 5,E
// - - - -
auto SET_0xEB = []()
{
	return SET_BIT_IN_REG(5, E);
};

// 0xEC SET 5,H
// - - - -
auto SET_0xEC = []()
{
	return SET_BIT_IN_REG(5, H);
};

// 0xED SET 5,L
// - - - -
auto SET_0xED = []()
{
	return SET_BIT_IN_REG(5, L);
};

// 0xEE SET 5,(HL)
// - - - -
auto SET_0xEE = []()
{
	const auto SET_REG_WITH_5 = [](auto& reg) { return SET_BIT_IN_REG(5, reg); };
	return RUN_COMMAND_ON_ADDRESS(HL_const, SET_REG_WITH_5);
};

// 0xEF SET 5,A
// - - - -
auto SET_0xEF = []()
{
	return SET_BIT_IN_REG(5, A);
};

// 0xF0 SET 6,B
// - - - -
auto SET_0xF0 = []()
{
	return SET_BIT_IN_REG(6, B);
};

// 0xF1 SET 6,C
// - - - -
auto SET_0xF1 = []()
{
	return SET_BIT_IN_REG(6, C);
};

// 0xF2 SET 6,D
// - - - -
auto SET_0xF2 = []()
{
	return SET_BIT_IN_REG(6, D);
};

// 0xF3 SET 6,E
// - - - -
auto SET_0xF3 = []()
{
	return SET_BIT_IN_REG(6, E);
};

// 0xF4 SET 6,H
// - - - -
auto SET_0xF4 = []()
{
	return SET_BIT_IN_REG(6, H);
};

// 0xF5 SET 6,L
// - - - -
auto SET_0xF5 = []()
{
	return SET_BIT_IN_REG(6, L);
};

// 0xF6 SET 6,(HL)
// - - - -
auto SET_0xF6 = []()
{
	const auto SET_REG_WITH_6 = [](auto& reg) { return SET_BIT_IN_REG(6, reg); };
	return RUN_COMMAND_ON_ADDRESS(HL_const, SET_REG_WITH_6);
};

// 0xF7 SET 6,A
// - - - -
auto SET_0xF7 = []()
{
	return SET_BIT_IN_REG(6, A);
};

// 0xF8 SET 7,B
// - - - -
auto SET_0xF8 = []()
{
	return SET_BIT_IN_REG(7, B);
};

// 0xF9 SET 7,C
// - - - -
auto SET_0xF9 = []()
{
	return SET_BIT_IN_REG(7, C);
};

// 0xFA SET 7,D
// - - - -
auto SET_0xFA = []()
{
	return SET_BIT_IN_REG(7, D);
};

// 0xFB SET 7,E
// - - - -
auto SET_0xFB = []()
{
	return SET_BIT_IN_REG(7, E);
};

// 0xFC SET 7,H
// - - - -
auto SET_0xFC = []()
{
	return SET_BIT_IN_REG(7, H);
};

// 0xFD SET 7,L
// - - - -
auto SET_0xFD = []()
{
	return SET_BIT_IN_REG(7, L);
};

// 0xFE SET 7,(HL)
// - - - -
auto SET_0xFE = []()
{
	const auto SET_REG_WITH_7 = [](auto& reg) { return SET_BIT_IN_REG(7, reg); };
	return RUN_COMMAND_ON_ADDRESS(HL_const, SET_REG_WITH_7);
};

// 0xFF SET 7,A
// - - - -
auto SET_0xFF = []()
{
	return SET_BIT_IN_REG(7, A);
};
} // Core

#endif // __LR35902_SET_H__