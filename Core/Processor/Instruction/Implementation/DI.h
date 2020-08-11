/**
 * @file        DI.h
 * @author      Noam Rodrik
 * @description Implementation to the DI instruction(s).
 */
#ifndef __LR35902_DI_H__
#define __LR35902_DI_H__

#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>
#include <Core/Interrupts/Flags/IME.h>

namespace Core
{
// 0xF3 DI
// - - - -
static constexpr auto DI_0xF3 = []()
{
	// Changes memory.
	IME::DisableInterrupts();
	return true;
};
} // Core

#endif // __LR35902_DI_H__