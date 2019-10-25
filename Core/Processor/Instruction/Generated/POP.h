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
	SP.Pop(B);
	SP.Pop(C);
	return true;
};

// 0xD1 POP DE
// - - - -
auto POP_0xD1 = []()
{
	SP.Pop(D);
	SP.Pop(E);
	return true;
};

// 0xE1 POP HL
// - - - -
auto POP_0xE1 = []()
{
	SP.Pop(H);
	SP.Pop(L);
	return true;
};

// 0xF1 POP AF
// - - - -
auto POP_0xF1 = []()
{
	SP.Pop(A);
	SP.Pop(F);
	return true;
};
} // Core

#endif // __LR35902_POP_H__