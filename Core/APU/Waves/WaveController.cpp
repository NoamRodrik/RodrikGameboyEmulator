/**
 * @file		WaveController.cpp
 * @author		Noam Rodrik
 * @description Main logic of the APU's wave controller.
 */
#include <Core/APU/Waves/WaveController.h>
#include <Core/APU/Definitions.h>
#include <Core/APU/Waves/Implementation/SoundChannelNoise.h>
#include <Core/APU/Waves/Implementation/SoundChannel1.h>
#include <Core/APU/Waves/Implementation/SoundChannel2.h>
#include <Core/APU/Waves/Implementation/SoundChannel3.h>

using namespace API;

namespace Core
{
WaveController::WaveController()
{
	// Initializing the channels
	this->_waves[0] = std::make_unique<SoundChannel1>();
	this->_waves[1] = std::make_unique<SoundChannel2>();
	this->_waves[2] = std::make_unique<SoundChannel3>();
	this->_waves[3] = std::make_unique<SoundChannelNoise>();
}

const float_t WaveController::Sample(const OutputTerminal output, const float_t global_time, const float_t time_step, const float_t volume)
{
	float_t sample{0.0f};

	for (const auto& wave : this->_waves)
	{
		if (wave->IsEnabled() && wave->Activated(output))
		{
			sample += BASE_AMPLITUDE * volume * wave->Sample(global_time);
		}
	}

	return sample;
}

ISoundChannel* WaveController::GetWave(const SoundChannel type)
{
	SANITY(static_cast<std::size_t>(type) - 1 < this->_waves.size(), "Bad sound channel");
	return this->_waves[static_cast<std::size_t>(type) - 1].get();
}
} // Core