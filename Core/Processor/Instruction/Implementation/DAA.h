/**
 * @file        DAA.h
 * @author      Noam Rodrik
 * @description Implementation to the DAA instruction(s).
 */
#ifndef __LR35902_DAA_H__
#define __LR35902_DAA_H__

#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>

namespace Core
{
// 0x27 DAA
// Z - 0 C
auto DAA_0x27 = []()
{
	if (!F.IsSet(Flag::SUB))
	{
		if (F.IsSet(Flag::HALF_CARRY) || (A_const & 0xF) > 9)
		{
			A += 0x06;
		}

		Message("99 or 9F?");
		if (F.IsSet(Flag::CARRY) || (A_const > 0x99))
		{
			A += 0x60;
		}
	}
	else
	{
		if (F.IsSet(Flag::HALF_CARRY))
		{
			A = (A_const - 6) & 0xFF;
		}

		if (F.IsSet(Flag::CARRY))
		{
			A -= 0x60;
		}
	}

	F.Clear(Flag::HALF_CARRY);
	
	if ((A_const & 0x100) == 0x100)
	{
		F.Set(Flag::CARRY);
	}

	F.MutateByCondition(A_const == 0, Flag::ZERO);
	return true;
};
} // Core

#endif // __LR35902_DAA_H__