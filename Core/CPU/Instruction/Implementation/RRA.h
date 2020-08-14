/**
 * @file        RRA.h
 * @author      Noam Rodrik
 * @description Implementation to the RRA instruction(s).
 */
#ifndef __LR35902_RRA_H__
#define __LR35902_RRA_H__

#include <Core/CPU/Processor.h>
#include <Core/CPU/Instruction/Shortcuts.h>
#include <Core/CPU/Instruction/Prefix/Implementation/RR.h>

using namespace API;

namespace Core
{
// 0x1F RRA
// 0 0 0 C
static constexpr auto RRA_0x1F = []()
{
	const bool RESULT = RR_0x1F();
	F.Clear(Flag::ZERO);
	return RESULT;
};
} // Core

#endif // __LR35902_RRA_H__