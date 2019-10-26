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
	IP = D16();

	// Don't change the IP.
	return false;
};

// 0xC4 CALL NZ,a16
// - - - -
auto CALL_0xC4 = []()
{
	return !F.IsSet(Flag::ZERO) ? CALL_A16() : true;
};

// 0xCC CALL Z,a16
// - - - -
auto CALL_0xCC = []()
{
	return F.IsSet(Flag::ZERO) ? CALL_A16() : true;
};

// 0xCD CALL a16
// - - - -
auto CALL_0xCD = []()
{
	return CALL_A16();
};

// 0xD4 CALL NC,a16
// - - - -
auto CALL_0xD4 = []()
{
	return !F.IsSet(Flag::CARRY) ? CALL_A16() : true;
};

// 0xDC CALL C,a16
// - - - -
auto CALL_0xDC = []()
{
	return F.IsSet(Flag::CARRY) ? CALL_A16() : true;
};
} // Core

#endif // __LR35902_CALL_H__