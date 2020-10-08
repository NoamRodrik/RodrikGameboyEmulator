/**
 * @file		Definitions.h
 * @author		Noam Rodrik
 * @description LR35902 APU definitions
 */
#ifndef __LR35902_APU_DEFINITIONS_H__
#define __LR35902_APU_DEFINITIONS_H__

#include <API/Definitions.h>
#include <Tools/Tools.h>

namespace Core
{
enum class [[nodiscard]] OutputTerminal
{
	// Right
	SO1 = 0,

	// Left
	SO2 = 1
};

enum class [[nodiscard]] SoundChannel
{
	PULSE_A = 1,
	PULSE_B = 2,
	WAVE = 3,
	NOISE = 4
};
} // Core

#endif // __LR35902_APU_DEFINITIONS_H__