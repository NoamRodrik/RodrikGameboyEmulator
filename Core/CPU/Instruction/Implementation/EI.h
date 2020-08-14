/**
 * @file        EI.h
 * @author      Noam Rodrik
 * @description Implementation to the EI instruction(s).
 */
#ifndef __LR35902_EI_H__
#define __LR35902_EI_H__

#include <Core/CPU/Interrupts/SpecialRegisters/IME.h>
#include <Core/CPU/Instruction/Shortcuts.h>
#include <Core/CPU/Processor.h>

using namespace API;

namespace Core
{
// 0xFB EI
// - - - -
static constexpr auto EI_0xFB = []()
{
	// Changes IME.
	IME::Schedule();
	return true;
};
} // Core

#endif // __LR35902_EI_H__