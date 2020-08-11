/**
 * @file        HALT.h
 * @author      Noam Rodrik
 * @description Implementation to the HALT instruction(s).
 */
#ifndef __LR35902_HALT_H__
#define __LR35902_HALT_H__

#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>

namespace Core
{
// 0x76 HALT
// - - - -
static constexpr auto HALT_0x76 = []()
{
	Processor::Halt();
	return true;
};
} // Core

#endif // __LR35902_HALT_H__