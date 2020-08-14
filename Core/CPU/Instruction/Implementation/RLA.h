/**
 * @file        RLA.h
 * @author      Noam Rodrik
 * @description Implementation to the RLA instruction(s).
 */
#ifndef __LR35902_RLA_H__
#define __LR35902_RLA_H__

#include <Core/CPU/Processor.h>
#include <Core/CPU/Instruction/Shortcuts.h>
#include <Core/CPU/Instruction/Prefix/Implementation/RL.h>

namespace Core
{
// 0x17 RLA
// 0 0 0 C
static constexpr auto RLA_0x17 = []()
{
	const bool RESULT = RL_0x17();
	F.Clear(Flag::ZERO);
	return RESULT;
};
} // Core

#endif // __LR35902_RLA_H__