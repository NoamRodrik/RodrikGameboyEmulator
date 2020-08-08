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
		// An addition has occurred.
		const bool convert_carry_flag = F.IsSet(Flag::CARRY) || (A_const > 0x99);
		const bool convert_half_carry_flag = F.IsSet(Flag::HALF_CARRY) || ((A_const & 0x0F) > 0x09);
		F.MutateByCondition(convert_carry_flag, Flag::CARRY);
		A += (convert_carry_flag * 0x60 + convert_half_carry_flag * 0x6);
	}
	else
	{
		A -= (F.IsSet(Flag::CARRY) * 0x60 + F.IsSet(Flag::HALF_CARRY) * 0x06);
	}

	F.MutateByCondition(A_const == 0, Flag::ZERO);
	F.Clear(Flag::HALF_CARRY);
	return true;
};
/*
auto DAA_0x27 = []()
{
	address_t daa_correction = F.IsSet(Flag::CARRY) ? 0x60 : 0x00;

	if (F.IsSet(Flag::HALF_CARRY) || (!F.IsSet(Flag::SUB) && (static_cast<const data_t>(A_const) & 0x0F) > 9))
	{
		daa_correction |= 0x06;
	}

	if (F.IsSet(Flag::CARRY) || (!F.IsSet(Flag::SUB) && static_cast<const data_t>(A_const) > 0x99))
	{
		daa_correction |= 0x60;
	}

	A = F.IsSet(Flag::SUB) ? static_cast<const data_t>(A_const) - daa_correction :
							 static_cast<const data_t>(A_const) + daa_correction;


	F.MutateByCondition(((daa_correction << 2) & 0x100) != 0, Flag::CARRY);
	F.MutateByCondition(static_cast<const data_t>(A_const) == 0, Flag::ZERO);
	F.Clear(Flag::HALF_CARRY);

	return true;
};
*/
} // Core

#endif // __LR35902_DAA_H__