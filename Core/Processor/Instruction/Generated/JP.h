/**
 * @file        JP.h
 * @author      Noam Rodrik
 * @description Implementation to the JP instruction(s).
 */
#ifndef __LR35902_JP_H__
#define __LR35902_JP_H__

#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>

namespace Core
{	
auto JP_ADDR = [](const auto& address)
{
	IP = DataAt(address + 1) << 8 | DataAt(address);

	// Don't change IP.
	return false;
};

auto JP_A16 = []()
{
	return JP_ADDR(IP);
};

// 0xC2 JP NZ,a16
// - - - -
auto JP_0xC2 = []()
{
	return !F.IsSet(Flag::ZERO) ? JP_A16() : true;
};

// 0xC3 JP a16
// - - - -
auto JP_0xC3 = []()
{
	return JP_A16();
};

// 0xCA JP Z,a16
// - - - -
auto JP_0xCA = []()
{
	return F.IsSet(Flag::ZERO) ? JP_A16() : true;
};

// 0xD2 JP NC,a16
// - - - -
auto JP_0xD2 = []()
{
	return !F.IsSet(Flag::CARRY) ? JP_A16() : true;
};

// 0xDA JP C,a16
// - - - -
auto JP_0xDA = []()
{
	return F.IsSet(Flag::CARRY) ? JP_A16() : true;
};

// 0xE9 JP (HL)
// - - - -
auto JP_0xE9 = []()
{
	return JP_ADDR(HL_const);
};
} // Core

#endif // __LR35902_JP_H__