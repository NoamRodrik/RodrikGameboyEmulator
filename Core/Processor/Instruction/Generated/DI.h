/**
 * @file        DI.h
 * @author      Noam Rodrik
 * @description Implementation to the DI instruction(s).
 */
#ifndef __LR35902_DI_H__
#define __LR35902_DI_H__

#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>
#include <Core/Interrupts/Registers/InterruptEnable.h>

namespace Core
{
// 0xF3 DI
// - - - -
auto DI_0xF3 = []()
{
	// Changes memory.
	InterruptEnable interrupt_enable = OFF;
	return true;
};
} // Core

#endif // __LR35902_DI_H__