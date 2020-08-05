/**
 * @file		Definitions.h
 * @author		Noam Rodrik
 * @description LR35902 main definition header.
 */
#ifndef __LR35902_DEFINITIONS_H__
#define __LR35902_DEFINITIONS_H__

#include <Core/API/Definitions.h>
#include <stdint.h>

namespace Core
{
	/* Flag Register Indexes */
	enum class Flag : uint8_t
	{
		ZERO = 7,
		SUB = 6,
		HALF_CARRY = 5,
		CARRY = 4
	};

	static_assert(static_cast<uint8_t>(Flag::ZERO) < CHAR_BIT &&
				  static_cast<uint8_t>(Flag::SUB) < CHAR_BIT &&
		          static_cast<uint8_t>(Flag::HALF_CARRY) < CHAR_BIT &&
		          static_cast<uint8_t>(Flag::CARRY) < CHAR_BIT);

#ifdef SKIP_BOOT
#undef SKIP_BOOT
#endif

#ifdef _DEBUG
	Message("Cancel this if you don't want to skip the CPU");
	#define SKIP_BOOT 1
#else
	#define SKIP_BOOT 0
#endif

} // Core

#endif // __LR35902_DEFINITIONS_H__
