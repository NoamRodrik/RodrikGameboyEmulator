/**
 * @file        DAA.h
 * @author      Noam Rodrik
 * @description Implementation to the DAA instruction(s).
 */
#ifndef __LR35902_DAA_H__
#define __LR35902_DAA_H__

#include <Core/CPU/Processor.h>
#include <Core/CPU/Instructions/Shortcuts.h>

using namespace API;

namespace Core
{
// 0x27 DAA
// Z - 0 C
static constexpr auto DAA_0x27 = []()
{
	data_t adjust{F.IsSet(Flag::CARRY) ? static_cast<data_t>(0x60) : static_cast<data_t>(0x00)};
	adjust |= F.IsSet(Flag::HALF_CARRY) ? 0x06 : 0x00;
	if (!F.IsSet(Flag::SUB))
	{
		if ((A_const & 0xF) > 9)
		{
			adjust |= 0x06;
		}

		if (A_const > 0x99)
		{
			adjust |= 0x60;
		}

		A += static_cast<const data_t>(adjust);
	}
	else
	{
		A -= static_cast<const data_t>(adjust);
	}

	F.MutateByCondition(A_const == 0, Flag::ZERO);
	F.Clear(Flag::HALF_CARRY);
	F.MutateByCondition(adjust >= 0x60, Flag::CARRY);

	return true;
};
} // Core

#endif // __LR35902_DAA_H__