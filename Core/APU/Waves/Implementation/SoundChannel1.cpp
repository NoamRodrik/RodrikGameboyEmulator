/**
 * @file		SoundChannel1.cpp
 * @author		Noam Rodrik
 * @description Main logic of the APU's sound channel 1.
 */
#include <Core/APU/Waves/Implementation/SoundChannel1.h>
#include <Core/APU/Registers/NR51.h>
#include <Core/APU/Definitions.h>

using namespace API;

namespace Core
{
const float_t SoundChannel1::Sample(const float_t time) const
{
	// Square wave
	Message("TODO");
	return 0;
}
} // Core