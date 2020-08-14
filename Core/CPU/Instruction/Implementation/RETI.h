/**
 * @file        RETI.h
 * @author      Noam Rodrik
 * @description Implementation to the RETI instruction(s).
 */
#ifndef __LR35902_RETI_H__
#define __LR35902_RETI_H__

#include <Core/CPU/Instruction/Shortcuts.h>
#include <Core/CPU/Interrupts/SpecialRegisters/IME.h>
#include <Core/CPU/Processor.h>
#include "EI.h"

using namespace API;

namespace Core
{
// 0xD9 RETI
// - - - -
static constexpr auto RETI_0xD9 = []()
{
	// Pop two bytes from stack & jump to that address then
	// enable interrupts
	RET();
	EI_0xFB();

	// Don't want to change PC!
	return false;
};
} // Core

#endif // __LR35902_RETI_H__