/**
 * @file        RET.h
 * @author      Noam Rodrik
 * @description Implementation to the RET instruction(s).
 */
#ifndef __LR35902_RET_H__
#define __LR35902_RET_H__

#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>

namespace Core
{
auto RET = []()
{
	SP.Pop(IP);

	// Returning it back for RET.
	IP -= 1;
};

// 0xC0 RET NZ
// - - - -
auto RET_0xC0 = []()
{
	if (!F.IsSet(Flag::ZERO))
	{
		RET();
	}
};

// 0xC8 RET Z
// - - - -
auto RET_0xC8 = []()
{
	if (F.IsSet(Flag::ZERO))
	{
		RET();
	}
};

// 0xC9 RET
// - - - -
auto RET_0xC9 = []()
{
	RET();
};

// 0xD0 RET NC
// - - - -
auto RET_0xD0 = []()
{
	if (!F.IsSet(Flag::CARRY))
	{
		RET();
	}
};

// 0xD8 RET C
// - - - -
auto RET_0xD8 = []()
{
	if (F.IsSet(Flag::CARRY))
	{
		RET();
	}
};
} // Core

#endif // __LR35902_RET_H__