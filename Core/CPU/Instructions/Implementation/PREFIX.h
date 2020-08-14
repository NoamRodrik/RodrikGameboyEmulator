/**
 * @file        PREFIX.h
 * @author      Noam Rodrik
 * @description Implementation to the PREFIX instruction(s).
 */
#ifndef __LR35902_PREFIX_H__
#define __LR35902_PREFIX_H__

#include <Core/CPU/Processor.h>
#include <Core/CPU/Instructions/Shortcuts.h>

using namespace API;

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