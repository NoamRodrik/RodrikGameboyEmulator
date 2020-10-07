/**
 * @file		APU.cpp
 * @author		Noam Rodrik
 * @description Main logic of the APU
 */
#include <Core/APU/APU.h>
#include <Core/APU/Definitions.h>

#include <Core/APU/Registers/NR50.h>
#include <Core/APU/Registers/NR52.h>

namespace Core
{
float_t APU::SoundDemultiplexer(int32_t channel, float_t global_time, float_t time_step)
{
	SANITY(channel >= static_cast<int32_t>(OutputTerminal::SO1) && channel <= static_cast<int32_t>(OutputTerminal::SO2),
		   "Got invalid values for the channel");

	// Checking if APU device is on.
	NR52 nr52{};
	if (!nr52.IsSoundEnabled())
	{
		return 0;
	}

	return APU::Play(static_cast<OutputTerminal>(channel), global_time, time_step);
}

const float_t APU::Sample(const OutputTerminal output, const float_t global_time, const float_t time_step, const API::data_t volume)
{
	switch (output)
	{
		// Normal channels
		case OutputTerminal::SO1:
		{
			// This should be right.
			Message("TEST!");
			//return APU::VolumeWaveManipulator(volume) * sin(global_time * 440.0f * 2.0f * atan(1) * 4);
			break;
		}
	
		case OutputTerminal::SO2:
		{
			// This should be left.
			Message("TEST!");
			//return APU::VolumeWaveManipulator(volume) * sin(global_time * 349.228f * 2.0f * atan(1) * 4);
			break;
		}
	}

	return 0;
}

const float_t APU::Play(const OutputTerminal output, const float_t global_time, const float_t time_step)
{
	// Checking if the specific output terminal is enabled.
	NR50 nr50{};
	const API::data_t OUTPUT_VOLUME{nr50.MasterVolumeLevel(output)};

	return APU::Sample(output, global_time, time_step, OUTPUT_VOLUME);
}

const float_t APU::VolumeWaveManipulator(const API::data_t volume)
{
	// The volume of a wave is simply the intensity of the wave.
	// For example, consider this wave as the maximum intensity sin wave:
	//     . .    
	//   .     . 
	// .        .     .             <-------------- sin(x)
	//           .   .
	//            . .
	//
	// A nullified wave (0 volume!) will *flatten*, and will draw as such:
	// (VOLUME / MAX_VOLUME) * sin(x)
	// So for a low value of VOLUME, the sin wave will converge into a
	// constant line:
	//
	//  . . . .
	// .        .        .        <----------------- (1/n) * sin(x)
	//            . . . .
	//
	// Down to zero:
	//
	//
	// . . . . . . . . . . . .   <------------------- 0 * sin(x)
	//
	//
	return (volume / APU::MAX_VOLUME);
}
} // Core