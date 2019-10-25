/**
 * @file        CCF.h
 * @author      Noam Rodrik
 * @description Implementation to the CCF instruction(s).
 */
#ifndef __LR35902_CCF_H__
#define __LR35902_CCF_H__

#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>

namespace Core
{
// 0x3F CCF
// - 0 0 C
auto CCF_0x3F = []()
{
	F.Clear(Flag::HALF_CARRY);
	F.Clear(Flag::SUB);
	F.MutateByCondition(F.IsSet(Flag::CARRY), Flag::CARRY);
};
} // Core

#endif // __LR35902_CCF_H__