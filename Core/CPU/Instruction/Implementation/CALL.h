/**
 * @file        CALL.h
 * @author      Noam Rodrik
 * @description Implementation to the CALL instruction(s).
 */
#ifndef __LR35902_CALL_H__
#define __LR35902_CALL_H__

#include <Core/CPU/Processor.h>
#include <Core/CPU/Instruction/Shortcuts.h>

using namespace API;

namespace Core
{
static constexpr auto CALL = []()
{
	// + 3 Because we want to skip the call size.
	SP.Push(static_cast<const address_t>(PC_const + 3));
	PC = A16();

	// Don't change the PC.
	return false;
};

// 0xC4 CALL NZ,a16
// - - - -
static constexpr auto CALL_0xC4 = []()
{
	return !F.IsSet(Flag::ZERO) ? CALL() : true;
};

// 0xCC CALL Z,a16
// - - - -
static constexpr auto CALL_0xCC = []()
{
	return F.IsSet(Flag::ZERO) ? CALL() : true;
};

// 0xCD CALL a16
// - - - -
static constexpr auto CALL_0xCD = []()
{
	return CALL();
};

// 0xD4 CALL NC,a16
// - - - -
static constexpr auto CALL_0xD4 = []()
{
	return !F.IsSet(Flag::CARRY) ? CALL() : true;
};

// 0xDC CALL C,a16
// - - - -
static constexpr auto CALL_0xDC = []()
{
	return F.IsSet(Flag::CARRY) ? CALL() : true;
};
} // Core

#endif // __LR35902_CALL_H__