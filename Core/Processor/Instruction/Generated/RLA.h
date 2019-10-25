/**
 * @file        RLA.h
 * @author      Noam Rodrik
 * @description Implementation to the RLA instruction(s).
 */
#ifndef __LR35902_RLA_H__
#define __LR35902_RLA_H__

#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>

namespace Core
{
// 0x17 RLA
// 0 0 0 C
auto RLA_0x17 = []()
{
	F.Clear(Flag::ZERO);
	F.Clear(Flag::SUB);
	F.Clear(Flag::HALF_CARRY);

	// Fetching the utmost left bit.
	const uint8_t shifted_bit = (A & 0x80) >> 7;
	const uint8_t already_carried_bit = F.IsSet(Flag::CARRY) & 0x01;
	F.MutateByCondition(shifted_bit == 0x01, Flag::CARRY);

	// Rotate left A.
	// The shifted bit goes into the carry flag, the carry flag goes into A.
	A <<= 1;
	A |= already_carried_bit;
	return true;
};
} // Core

#endif // __LR35902_RLA_H__