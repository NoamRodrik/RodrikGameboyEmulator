/**
 * @file        STOP.h
 * @author      Noam Rodrik
 * @description Implementation to the STOP instruction(s).
 */
#ifndef __LR35902_STOP_H__
#define __LR35902_STOP_H__

#include <Core/CPU/Processor.h>
#include <Core/CPU/Instructions/Shortcuts.h>

using namespace API;

namespace Core
{
// 0x10 STOP
// - - - -
static constexpr auto STOP_0x10 = []()
{
	Processor::Stop();
	return true;
};
} // Core

#endif // __LR35902_STOP_H__