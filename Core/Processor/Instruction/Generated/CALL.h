/**
 * @file        CALL.h
 * @author      Noam Rodrik
 * @description Implementation to the CALL instruction(s).
 */
#ifndef __LR35902_CALL_H__
#define __LR35902_CALL_H__

#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>

namespace Core
{
auto CALL_A16 = []()
{
	SP.Push(IP);
	IP = DataAt(IP_const + 2) << 8 | DataAt(IP_const + 1);

	// Returning it back for CALL.
	IP -= 3;
};

// 0xC4 CALL NZ,a16
// - - - -
auto CALL_0xC4 = []()
{
	if (!F.IsSet(Flag::ZERO))
	{
		CALL_A16();
	}
};

// 0xCC CALL Z,a16
// - - - -
auto CALL_0xCC = []()
{
	if (F.IsSet(Flag::ZERO))
	{
		CALL_A16();
	}
};

// 0xCD CALL a16
// - - - -
auto CALL_0xCD = []()
{
	CALL_A16();
};

// 0xD4 CALL NC,a16
// - - - -
auto CALL_0xD4 = []()
{
	if (!F.IsSet(Flag::CARRY))
	{
		CALL_A16();
	}
};

// 0xDC CALL C,a16
// - - - -
auto CALL_0xDC = []()
{
	if (F.IsSet(Flag::CARRY))
	{
		CALL_A16();
	}
};
} // Core

#endif // __LR35902_CALL_H__