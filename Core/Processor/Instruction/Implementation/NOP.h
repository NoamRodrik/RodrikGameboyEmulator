/**
 * @file		NOP.h
 * @author		Noam Rodrik
 * @description Implementation to the NOP instruction(s).
 */
#ifndef __LR35902_NOP_H__
#define __LR35902_NOP_H__

#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>

namespace Core
{
// 0x00 NOP
// - - - -
static constexpr auto NOP_0x00 = []()
{
	return true;
};
} // Core

#endif // __LR35902_NOP_H__