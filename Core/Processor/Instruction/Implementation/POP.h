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
auto POP_0xC1 = []()
{
	SP.Pop(C);
	SP.Pop(B);
	return true;
};

// 0xD1 POP DE
// - - - -
auto POP_0xD1 = []()
{
	SP.Pop(E);
	SP.Pop(D);
	return true;
};

// 0xE1 POP HL
// - - - -
auto POP_0xE1 = []()
{
	SP.Pop(L);
	SP.Pop(H);
	return true;
};

// 0xF1 POP AF
// - - - -
auto POP_0xF1 = []()
{
	SP.Pop(F);
	SP.Pop(A);

	/* This is a feature of the EFL register: if overwritten, the lower 4 bits are always 0 nonetheless! */
	F = static_cast<data_t>(F_const) & 0xF0;
	return true;
};
} // Core

#endif // __LR35902_POP_H__