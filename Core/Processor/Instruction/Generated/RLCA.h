/**
 * @file        RLCA.h
 * @author      Noam Rodrik
 * @description Implementation to the RLCA instruction(s).
 */
#ifndef __LR35902_RLCA_H__
#define __LR35902_RLCA_H__

#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>

namespace Core
{
// 0x07 RLCA
// 0 0 0 C
auto RLCA_0x07 = []()
{
	F.Clear(Flag::ZERO);
	F.Clear(Flag::SUB);
	F.Clear(Flag::HALF_CARRY);

	// Fetching the utmost left bit.
	const uint8_t shifted_bit = (A & 0x80) >> 7;
	F.MutateByCondition(shifted_bit == 0x01, Flag::CARRY);

	// Rotate left A.
	// Restore shifted_bit's position.
	A <<= 1;
	A |= shifted_bit;
};
} // Core

#endif // __LR35902_RLCA_H__