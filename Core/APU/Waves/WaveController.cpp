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
#include <Core/APU/Registers/NR30.h>
#include <Core/APU/Registers/NR50.h>

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

ISoundChannel* WaveController::GetWave(const SoundChannel type)
{
	SANITY(static_cast<std::size_t>(type) - 1 < this->_waves.size(), "Bad sound channel");
	return this->_waves[static_cast<std::size_t>(type) - 1].get();
}

void WaveController::Clock(const uint8_t clocks)
{
	this->InitiateWaveChannel();

	this->LengthClock(clocks);
	this->EnvelopeClock(clocks);
	this->SweepClock(clocks);
	this->ChannelClock(clocks);
	this->SamplesClock(clocks);
}

void WaveController::InitiateWaveChannel()
{
	// SoundChannel3 has a specific case here.
	if (NR30{}.IsSoundOn())
	{
		static_cast<SoundChannel3*>(this->_waves[2].get())->Start();
	}
	else
	{
		static_cast<SoundChannel3*>(this->_waves[2].get())->Stop();
	}
}

void WaveController::LengthClock(const uint8_t clocks)
{
	this->_length_clocks += clocks;

	if (this->_length_clocks >= API::LR35902_HZ_CLOCK_256HZ)
	{
		this->_length_clocks -= API::LR35902_HZ_CLOCK_256HZ;

		for (auto&& wave : this->_waves)
		{
			wave->LengthTick();
		}
	}
}

void WaveController::SweepClock(const uint8_t clocks)
{
	this->_sweep_clocks += clocks;
	
	if (this->_sweep_clocks >= LR35902_HZ_CLOCK_128HZ)
	{
		this->_sweep_clocks -= LR35902_HZ_CLOCK_128HZ;

		for (auto&& wave : this->_waves)
		{
			wave->SweepTick();
		}
	}
}

void WaveController::EnvelopeClock(const uint8_t clocks)
{
	this->_envelope_clocks += clocks;

	if (this->_envelope_clocks >= LR35902_HZ_CLOCK_64HZ)
	{
		this->_envelope_clocks -= LR35902_HZ_CLOCK_64HZ;

		for (auto&& wave : this->_waves)
		{
			wave->EnvelopeTick();
		}
	}
}

void WaveController::ChannelClock(const uint8_t clocks)
{
	for (auto&& wave : this->_waves)
	{
		wave->Clock(clocks);
	}
}

void WaveController::SamplesClock(const uint8_t clocks)
{
	this->_samples_clocks += clocks;
	if (this->_samples_clocks >= API::AUDIO_SAMPLE_IN_CYCLES)
	{
		this->_samples_clocks -= API::AUDIO_SAMPLE_IN_CYCLES;

		const NR50 nr50{};
		const API::data_t VOLUME_RIGHT{nr50.MasterVolumeLevel(OutputTerminal::SO1)};
		const API::data_t VOLUME_LEFT{nr50.MasterVolumeLevel(OutputTerminal::SO2)};

		// Setting the sample data buffer
		this->_samples.SoundChannel1Right[this->_current_sample_counter] = this->_waves[0]->Activated(OutputTerminal::SO1) ? 
																		   this->_waves[0]->GetCurrentSample() / VOLUME_RIGHT : 0;
		this->_samples.SoundChannel2Right[this->_current_sample_counter] = this->_waves[1]->Activated(OutputTerminal::SO1) ? 
																		   this->_waves[1]->GetCurrentSample() / VOLUME_RIGHT : 0;
		this->_samples.SoundChannel3Right[this->_current_sample_counter] = this->_waves[2]->Activated(OutputTerminal::SO1) ? 
																		   this->_waves[2]->GetCurrentSample() / VOLUME_RIGHT : 0;
		this->_samples.SoundChannelNoiseRight[this->_current_sample_counter] = this->_waves[3]->Activated(OutputTerminal::SO1) ? 
																		   this->_waves[3]->GetCurrentSample() / VOLUME_RIGHT : 0;
		this->_samples.SoundChannel1Left[this->_current_sample_counter] = this->_waves[0]->Activated(OutputTerminal::SO2) ? 
																		   this->_waves[0]->GetCurrentSample() / VOLUME_LEFT : 0;
		this->_samples.SoundChannel2Left[this->_current_sample_counter] = this->_waves[1]->Activated(OutputTerminal::SO2) ?
																		   this->_waves[1]->GetCurrentSample() / VOLUME_LEFT : 0;
		this->_samples.SoundChannel3Left[this->_current_sample_counter] = this->_waves[2]->Activated(OutputTerminal::SO2) ?
																		   this->_waves[2]->GetCurrentSample() / VOLUME_LEFT : 0;
		this->_samples.SoundChannelNoiseLeft[this->_current_sample_counter] = this->_waves[3]->Activated(OutputTerminal::SO2) ?
																		   this->_waves[3]->GetCurrentSample() / VOLUME_LEFT : 0;

		this->_current_sample_counter += 1;

		if (this->_current_sample_counter == API::BUFFER_FRAMES)
		{
			DAC::GetInstance().FeedSamples(this->_samples);
			this->_current_sample_counter = 0x00;
		}
	}
}
} // Core