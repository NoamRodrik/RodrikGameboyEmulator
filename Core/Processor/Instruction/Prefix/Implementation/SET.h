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
// SET index, data
static constexpr auto SET = [](const uint8_t index, auto& data)
{
	SANITY(index <= 7, "Got an invalid bit index!");
	Tools::SetBit(data, index);
	return true;
};

// 0xC0 SET 0,B
// - - - -
static constexpr auto SET_0xC0 = []()
{
	return SET(0, B);
};

// 0xC1 SET 0,C
// - - - -
static constexpr auto SET_0xC1 = []()
{
	return SET(0, C);
};

// 0xC2 SET 0,D
// - - - -
static constexpr auto SET_0xC2 = []()
{
	return SET(0, D);
};

// 0xC3 SET 0,E
// - - - -
static constexpr auto SET_0xC3 = []()
{
	return SET(0, E);
};

// 0xC4 SET 0,H
// - - - -
static constexpr auto SET_0xC4 = []()
{
	return SET(0, H);
};

// 0xC5 SET 0,L
// - - - -
static constexpr auto SET_0xC5 = []()
{
	return SET(0, L);
};

// 0xC6 SET 0,(HL)
// - - - -
static constexpr auto SET_0xC6 = []()
{
	return RUN_COMMAND_ON_ADDRESS(HL_const, [](auto& reg) { return SET(0, reg); });
};

// 0xC7 SET 0,A
// - - - -
static constexpr auto SET_0xC7 = []()
{
	return SET(0, A);
};

// 0xC8 SET 1,B
// - - - -
static constexpr auto SET_0xC8 = []()
{
	return SET(1, B);
};

// 0xC9 SET 1,C
// - - - -
static constexpr auto SET_0xC9 = []()
{
	return SET(1, C);
};

// 0xCA SET 1,D
// - - - -
static constexpr auto SET_0xCA = []()
{
	return SET(1, D);
};

// 0xCB SET 1,E
// - - - -
static constexpr auto SET_0xCB = []()
{
	return SET(1, E);
};

// 0xCC SET 1,H
// - - - -
static constexpr auto SET_0xCC = []()
{
	return SET(1, H);
};

// 0xCD SET 1,L
// - - - -
static constexpr auto SET_0xCD = []()
{
	return SET(1, L);
};

// 0xCE SET 1,(HL)
// - - - -
static constexpr auto SET_0xCE = []()
{
	return RUN_COMMAND_ON_ADDRESS(HL_const, [](auto& reg) { return SET(1, reg); });
};

// 0xCF SET 1,A
// - - - -
static constexpr auto SET_0xCF = []()
{
	return SET(1, A);
};

// 0xD0 SET 2,B
// - - - -
static constexpr auto SET_0xD0 = []()
{
	return SET(2, B);
};

// 0xD1 SET 2,C
// - - - -
static constexpr auto SET_0xD1 = []()
{
	return SET(2, C);
};

// 0xD2 SET 2,D
// - - - -
static constexpr auto SET_0xD2 = []()
{
	return SET(2, D);
};

// 0xD3 SET 2,E
// - - - -
static constexpr auto SET_0xD3 = []()
{
	return SET(2, E);
};

// 0xD4 SET 2,H
// - - - -
static constexpr auto SET_0xD4 = []()
{
	return SET(2, H);
};

// 0xD5 SET 2,L
// - - - -
static constexpr auto SET_0xD5 = []()
{
	return SET(2, L);
};

// 0xD6 SET 2,(HL)
// - - - -
static constexpr auto SET_0xD6 = []()
{
	return RUN_COMMAND_ON_ADDRESS(HL_const, [](auto& reg) { return SET(2, reg); });
};

// 0xD7 SET 2,A
// - - - -
static constexpr auto SET_0xD7 = []()
{
	return SET(2, A);
};

// 0xD8 SET 3,B
// - - - -
static constexpr auto SET_0xD8 = []()
{
	return SET(3, B);
};

// 0xD9 SET 3,C
// - - - -
static constexpr auto SET_0xD9 = []()
{
	return SET(3, C);
};

// 0xDA SET 3,D
// - - - -
static constexpr auto SET_0xDA = []()
{
	return SET(3, D);
};

// 0xDB SET 3,E
// - - - -
static constexpr auto SET_0xDB = []()
{
	return SET(3, E);
};

// 0xDC SET 3,H
// - - - -
static constexpr auto SET_0xDC = []()
{
	return SET(3, H);
};

// 0xDD SET 3,L
// - - - -
static constexpr auto SET_0xDD = []()
{
	return SET(3, L);
};

// 0xDE SET 3,(HL)
// - - - -
static constexpr auto SET_0xDE = []()
{
	return RUN_COMMAND_ON_ADDRESS(HL_const, [](auto& reg) { return SET(3, reg); });
};

// 0xDF SET 3,A
// - - - -
static constexpr auto SET_0xDF = []()
{
	return SET(3, A);
};

// 0xE0 SET 4,B
// - - - -
static constexpr auto SET_0xE0 = []()
{
	return SET(4, B);
};

// 0xE1 SET 4,C
// - - - -
static constexpr auto SET_0xE1 = []()
{
	return SET(4, C);
};

// 0xE2 SET 4,D
// - - - -
static constexpr auto SET_0xE2 = []()
{
	return SET(4, D);
};

// 0xE3 SET 4,E
// - - - -
static constexpr auto SET_0xE3 = []()
{
	return SET(4, E);
};

// 0xE4 SET 4,H
// - - - -
static constexpr auto SET_0xE4 = []()
{
	return SET(4, H);
};

// 0xE5 SET 4,L
// - - - -
static constexpr auto SET_0xE5 = []()
{
	return SET(4, L);
};

// 0xE6 SET 4,(HL)
// - - - -
static constexpr auto SET_0xE6 = []()
{
	return RUN_COMMAND_ON_ADDRESS(HL_const, [](auto& reg) { return SET(4, reg); });
};

// 0xE7 SET 4,A
// - - - -
static constexpr auto SET_0xE7 = []()
{
	return SET(4, A);
};

// 0xE8 SET 5,B
// - - - -
static constexpr auto SET_0xE8 = []()
{
	return SET(5, B);
};

// 0xE9 SET 5,C
// - - - -
static constexpr auto SET_0xE9 = []()
{
	return SET(5, C);
};

// 0xEA SET 5,D
// - - - -
static constexpr auto SET_0xEA = []()
{
	return SET(5, D);
};

// 0xEB SET 5,E
// - - - -
static constexpr auto SET_0xEB = []()
{
	return SET(5, E);
};

// 0xEC SET 5,H
// - - - -
static constexpr auto SET_0xEC = []()
{
	return SET(5, H);
};

// 0xED SET 5,L
// - - - -
static constexpr auto SET_0xED = []()
{
	return SET(5, L);
};

// 0xEE SET 5,(HL)
// - - - -
static constexpr auto SET_0xEE = []()
{
	return RUN_COMMAND_ON_ADDRESS(HL_const, [](auto& reg) { return SET(5, reg); });
};

// 0xEF SET 5,A
// - - - -
static constexpr auto SET_0xEF = []()
{
	return SET(5, A);
};

// 0xF0 SET 6,B
// - - - -
static constexpr auto SET_0xF0 = []()
{
	return SET(6, B);
};

// 0xF1 SET 6,C
// - - - -
static constexpr auto SET_0xF1 = []()
{
	return SET(6, C);
};

// 0xF2 SET 6,D
// - - - -
static constexpr auto SET_0xF2 = []()
{
	return SET(6, D);
};

// 0xF3 SET 6,E
// - - - -
static constexpr auto SET_0xF3 = []()
{
	return SET(6, E);
};

// 0xF4 SET 6,H
// - - - -
static constexpr auto SET_0xF4 = []()
{
	return SET(6, H);
};

// 0xF5 SET 6,L
// - - - -
static constexpr auto SET_0xF5 = []()
{
	return SET(6, L);
};

// 0xF6 SET 6,(HL)
// - - - -
static constexpr auto SET_0xF6 = []()
{
	return RUN_COMMAND_ON_ADDRESS(HL_const, [](auto& reg) { return SET(6, reg); });
};

// 0xF7 SET 6,A
// - - - -
static constexpr auto SET_0xF7 = []()
{
	return SET(6, A);
};

// 0xF8 SET 7,B
// - - - -
static constexpr auto SET_0xF8 = []()
{
	return SET(7, B);
};

// 0xF9 SET 7,C
// - - - -
static constexpr auto SET_0xF9 = []()
{
	return SET(7, C);
};

// 0xFA SET 7,D
// - - - -
static constexpr auto SET_0xFA = []()
{
	return SET(7, D);
};

// 0xFB SET 7,E
// - - - -
static constexpr auto SET_0xFB = []()
{
	return SET(7, E);
};

// 0xFC SET 7,H
// - - - -
static constexpr auto SET_0xFC = []()
{
	return SET(7, H);
};

// 0xFD SET 7,L
// - - - -
static constexpr auto SET_0xFD = []()
{
	return SET(7, L);
};

// 0xFE SET 7,(HL)
// - - - -
static constexpr auto SET_0xFE = []()
{
	return RUN_COMMAND_ON_ADDRESS(HL_const, [](auto& reg) { return SET(7, reg); });
};

// 0xFF SET 7,A
// - - - -
static constexpr auto SET_0xFF = []()
{
	return SET(7, A);
};
} // Core

#endif // __LR35902_SET_H__