/**
 * @file		NOP.h
 * @author		Noam Rodrik
 * @description Implementation to the NOP instruction(s).
 */
#ifndef __LR35902_NOP_H__
#define __LR35902_NOP_H__

#include <Core/CPU/Processor.h>
#include <Core/CPU/Instruction/Shortcuts.h>

using namespace API;

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