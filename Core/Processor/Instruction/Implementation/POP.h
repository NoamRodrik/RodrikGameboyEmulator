/**
 * @file        POP.h
 * @author      Noam Rodrik
 * @description Implementation to the POP instruction(s).
 */
#ifndef __LR35902_POP_H__
#define __LR35902_POP_H__

#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>

namespace Core
{
// 0xC1 POP BC
// - - - -
static constexpr auto POP_0xC1 = []()
{
	SP.Pop(C);
	SP.Pop(B);
	return true;
};

// 0xD1 POP DE
// - - - -
static constexpr auto POP_0xD1 = []()
{
	SP.Pop(E);
	SP.Pop(D);
	return true;
};

// 0xE1 POP HL
// - - - -
static constexpr auto POP_0xE1 = []()
{
	SP.Pop(L);
	SP.Pop(H);
	return true;
};

// 0xF1 POP AF
// - - - -
static constexpr auto POP_0xF1 = []()
{
	SP.Pop(F);
	SP.Pop(A);

	/* This is a feature of the EFL register: if overwritten, the lower 4 bits are always 0 nonetheless! */
	F &= 0xF0;
	return true;
};
} // Core

#endif // __LR35902_POP_H__