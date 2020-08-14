/**
 * @file        CCF.h
 * @author      Noam Rodrik
 * @description Implementation to the CCF instruction(s).
 */
#ifndef __LR35902_CCF_H__
#define __LR35902_CCF_H__

#include <Core/CPU/Processor.h>
#include <Core/CPU/Instructions/Shortcuts.h>

using namespace API;

namespace Core
{
// 0x3F CCF
// - 0 0 C
static constexpr auto CCF_0x3F = []()
{
	F.Clear(Flag::SUB);
	F.Clear(Flag::HALF_CARRY);
	F.MutateByCondition(!F.IsSet(Flag::CARRY), Flag::CARRY);

	return true;
};
} // Core

#endif // __LR35902_CCF_H__