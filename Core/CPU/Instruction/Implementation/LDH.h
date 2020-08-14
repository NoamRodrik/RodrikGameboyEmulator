/**
 * @file        LDH.h
 * @author      Noam Rodrik
 * @description Implementation to the LDH instruction(s).
 */
#ifndef __LR35902_LDH_H__
#define __LR35902_LDH_H__

#include <Core/CPU/Processor.h>
#include <Core/CPU/Instruction/Shortcuts.h>

namespace Core
{
// 0xE0 LDH (a8),A
// - - - -
static constexpr auto LDH_0xE0 = []()
{
	return LD_ADDR(A8() + ZERO_PAGE_ADDRESS, A_const);
};

// 0xF0 LDH A,(a8)
// - - - -
static constexpr auto LDH_0xF0 = []()
{
	return LD(A, READ_DATA_AT(A8() + ZERO_PAGE_ADDRESS));
};
} // Core

#endif // __LR35902_LDH_H__