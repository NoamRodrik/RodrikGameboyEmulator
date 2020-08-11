/**
 * @file        RLCA.h
 * @author      Noam Rodrik
 * @description Implementation to the RLCA instruction(s).
 */
#ifndef __LR35902_RLCA_H__
#define __LR35902_RLCA_H__

#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>
#include <Core/Processor/Instruction/Prefix/Implementation/RLC.h>

namespace Core
{
// 0x07 RLCA
// 0 0 0 C
static constexpr auto RLCA_0x07 = []()
{
	const bool RESULT = RLC_0x07();
	F.Clear(Flag::ZERO);
	return RESULT;
};
} // Core

#endif // __LR35902_RLCA_H__