/**
 * @file        RRA.h
 * @author      Noam Rodrik
 * @description Implementation to the RRA instruction(s).
 */
#ifndef __LR35902_RRA_H__
#define __LR35902_RRA_H__

#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>

namespace Core
{
// 0x1F RRA
// 0 0 0 C
auto RRA_0x1F = []()
{
	F.Clear(Flag::ZERO);
	F.Clear(Flag::SUB);
	F.Clear(Flag::HALF_CARRY);

	// Fetching the utmost left bit.
	const uint8_t shifted_bit = A & 0x01;
	uint8_t already_carried_bit = F.IsSet(Flag::CARRY) & 0x01;
	F.MutateByCondition(shifted_bit == 0x01, Flag::CARRY);

	// Rotate right A.
	// The shifted bit goes into the carry flag, the carry flag goes into A.
	A >>= 1;
	already_carried_bit <<= 7;
	A |= already_carried_bit;
};
} // Core

#endif // __LR35902_RRA_H__