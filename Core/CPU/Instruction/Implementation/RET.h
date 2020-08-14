/**
 * @file        RET.h
 * @author      Noam Rodrik
 * @description Implementation to the RET instruction(s).
 */
#ifndef __LR35902_RET_H__
#define __LR35902_RET_H__

#include <Core/CPU/Processor.h>
#include <Core/CPU/Instruction/Shortcuts.h>

namespace Core
{
static constexpr auto RET = []()
{
	SP.Pop(PC);

	// Don't change the PC
	return false;
};

// 0xC0 RET NZ
// - - - -
static constexpr auto RET_0xC0 = []()
{
	return !F.IsSet(Flag::ZERO) ? RET() : true;
};

// 0xC8 RET Z
// - - - -
static constexpr auto RET_0xC8 = []()
{
	return F.IsSet(Flag::ZERO) ? RET() : true;
};

// 0xC9 RET
// - - - -
static constexpr auto RET_0xC9 = []()
{
	return RET();
};

// 0xD0 RET NC
// - - - -
static constexpr auto RET_0xD0 = []()
{
	return !F.IsSet(Flag::CARRY) ? RET() : true;
};

// 0xD8 RET C
// - - - -
static constexpr auto RET_0xD8 = []()
{
	return F.IsSet(Flag::CARRY) ? RET() : true;
};
} // Core

#endif // __LR35902_RET_H__