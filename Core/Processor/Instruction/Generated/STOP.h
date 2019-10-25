/**
 * @file        STOP.h
 * @author      Noam Rodrik
 * @description Implementation to the STOP instruction(s).
 */
#ifndef __LR35902_STOP_H__
#define __LR35902_STOP_H__

#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>

namespace Core
{
// 0x10 STOP
// - - - -
auto STOP_0x10 = []()
{
	Processor::GetInstance().Stop();
	return true;
};
} // Core

#endif // __LR35902_STOP_H__