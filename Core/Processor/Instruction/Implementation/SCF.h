/**
 * @file        SCF.h
 * @author      Noam Rodrik
 * @description Implementation to the SCF instruction(s).
 */
#ifndef __LR35902_SCF_H__
#define __LR35902_SCF_H__

#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>

namespace Core
{
// 0x37 SCF
// - 0 0 1
static constexpr auto SCF_0x37 = []()
{
	F.Clear(Flag::SUB);
	F.Clear(Flag::HALF_CARRY);
	F.Set(Flag::CARRY);

	return true;
};
} // Core

#endif // __LR35902_SCF_H__