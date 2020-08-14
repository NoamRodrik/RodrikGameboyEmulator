/**
 * @file        JP.h
 * @author      Noam Rodrik
 * @description Implementation to the JP instruction(s).
 */
#ifndef __LR35902_JP_H__
#define __LR35902_JP_H__

#include <Core/CPU/Processor.h>
#include <Core/CPU/Instructions/Shortcuts.h>

using namespace API;

namespace Core
{	
static constexpr auto JP = [](const address_t& address)
{
	PC = address;

	// Don't change PC.
	return false;
};

// 0xC2 JP NZ,a16
// - - - -
static constexpr auto JP_0xC2 = []()
{
	return !F.IsSet(Flag::ZERO) ? JP(A16()) : true;
};

// 0xC3 JP a16
// - - - -
static constexpr auto JP_0xC3 = []()
{
	return JP(A16());
};

// 0xCA JP Z,a16
// - - - -
static constexpr auto JP_0xCA = []()
{
	return F.IsSet(Flag::ZERO) ? JP(A16()) : true;
};

// 0xD2 JP NC,a16
// - - - -
static constexpr auto JP_0xD2 = []()
{
	return !F.IsSet(Flag::CARRY) ? JP(A16()) : true;
};

// 0xDA JP C,a16
// - - - -
static constexpr auto JP_0xDA = []()
{
	return F.IsSet(Flag::CARRY) ? JP(A16()) : true;
};

// 0xE9 JP HL
// - - - -
static constexpr auto JP_0xE9 = []()
{
	return JP(HL_const);
};
} // Core

#endif // __LR35902_JP_H__