/**
 * @file        PREFIX.h
 * @author      Noam Rodrik
 * @description Implementation to the PREFIX instruction(s).
 */
#ifndef __LR35902_PREFIX_H__
#define __LR35902_PREFIX_H__

#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>

namespace Core
{
// 0xCB PREFIX CB
// - - - -
static constexpr auto PREFIX_0xCB = []()
{
	Processor::SetPrefixCommand();
	return true;
};
} // Core

#endif // __LR35902_PREFIX_H__