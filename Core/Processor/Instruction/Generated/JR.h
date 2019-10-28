/**
 * @file        JR.h
 * @author      Noam Rodrik
 * @description Implementation to the JR instruction(s).
 */
#ifndef __LR35902_JR_H__
#define __LR35902_JR_H__

#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>

namespace Core
{
auto JR_R8 = []()
{
	PC += D8_TO_R8(DataAt(PC_const + 1)) + 2;

	// Don't change the PC!
	return false;
};

// 0x18 JR r8
// - - - -
auto JR_0x18 = []()
{
	return JR_R8();
};

// 0x20 JR NZ,r8
// - - - -
auto JR_0x20 = []()
{
	return !F.IsSet(Flag::ZERO) ? JR_R8() : true;
};

// 0x28 JR Z,r8
// - - - -
auto JR_0x28 = []()
{
	return F.IsSet(Flag::ZERO) ? JR_R8() : true;
};

// 0x30 JR NC,r8
// - - - -
auto JR_0x30 = []()
{
	return !F.IsSet(Flag::CARRY) ? JR_R8() : true;
};

// 0x38 JR C,r8
// - - - -
auto JR_0x38 = []()
{
	return F.IsSet(Flag::CARRY) ? JR_R8() : true;
};
} // Core

#endif // __LR35902_JR_H__