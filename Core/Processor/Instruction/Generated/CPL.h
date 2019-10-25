/**
 * @file        CPL.h
 * @author      Noam Rodrik
 * @description Implementation to the CPL instruction(s).
 */
#ifndef __LR35902_CPL_H__
#define __LR35902_CPL_H__

#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>

namespace Core
{
// 0x2F CPL
// - 1 1 -
auto CPL_0x2F = []()
{
	F.Set(Flag::SUB);
	F.Set(Flag::HALF_CARRY);

	// Flip A
	A = ~A;

	return true;
};
} // Core

#endif // __LR35902_CPL_H__