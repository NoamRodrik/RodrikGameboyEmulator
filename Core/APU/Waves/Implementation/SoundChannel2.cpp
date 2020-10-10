/**
 * @file		SoundChannel2.cpp
 * @author		Noam Rodrik
 * @description Main logic of the APU's sound channel 2.
 */
#include <Core/APU/Waves/Implementation/SoundChannel2.h>
#include <Core/APU/Registers/NR24.h>
#include <Core/APU/Registers/NR22.h>
#include <Core/APU/Registers/NR21.h>
#include <Core/APU/Definitions.h>

namespace Core
{
void SoundChannel2::Restart()
{
	ASoundChannelSquare::SquareChannelRestart<NR21, NR24, NR22>();
}

void SoundChannel2::UpdateSample()
{
	ASoundChannelSquare::SquareChannelUpdateSamples<NR21>();
}
} // Core