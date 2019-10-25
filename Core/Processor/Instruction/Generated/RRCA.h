/**
 * @file        RRCA.h
 * @author      Noam Rodrik
 * @description Implementation to the RRCA instruction(s).
 */
#ifndef __LR35902_RRCA_H__
#define __LR35902_RRCA_H__

#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>

namespace Core
{
// 0x0F RRCA
// 0 0 0 C
auto RRCA_0x0F = []()
{
	F.Clear(Flag::ZERO);
	F.Clear(Flag::SUB);
	F.Clear(Flag::HALF_CARRY);

	// Fetching the utmost left bit.
	uint8_t shifted_bit = A & 0x01;
	F.MutateByCondition(shifted_bit == 0x01, Flag::CARRY);

	// Rotate right A.
	// Restore shifted_bit's position.
	A >>= 1;
	shifted_bit <<= 7;
	A |= shifted_bit;
	return true;
};
} // Core

#endif // __LR35902_RRCA_H__