/**
 * @file		WaveController.h
 * @author		Noam Rodrik
 * @description The implementation for LR35902's synthesizer.
 */
#ifndef __CORE_WAVE_CONTROLLER_H__
#define __CORE_WAVE_CONTROLLER_H__

#include <Core/APU/Waves/ISoundChannel.h>
#include <Core/APU/Mechanics/DAC.h>
#include <Core/APU/Definitions.h>
#include <API/Definitions.h>
#include <cstddef>
#include <memory>
#include <array>

namespace Core
{
class [[nodiscard]] WaveController
{
public:
	WaveController();
	~WaveController() = default;

public:
	[[nodiscard]] ISoundChannel* GetWave(const SoundChannel type);
	void Clock(const uint8_t clocks);

private:
	void InitiateWaveChannel();
	void LengthClock(const uint8_t clocks);
	void SweepClock(const uint8_t clocks);
	void EnvelopeClock(const uint8_t clocks);
	void ChannelClock(const uint8_t clocks);
	void SamplesClock(const uint8_t clocks);

private:
	std::size_t                                                            _length_clocks{0x00};
	std::size_t                                                            _sweep_clocks{0x00};
	std::size_t                                                            _envelope_clocks{0x00};
	std::size_t                                                            _samples_clocks{0x00};
	std::size_t                                                            _current_sample_counter{0x00};
	std::array<std::unique_ptr<ISoundChannel>, API::AUDIO_CHANNELS_AMOUNT> _waves{};
	DAC::SampleData                                                        _samples{};
};
} // Core

#endif // __CORE_WAVE_CONTROLLER_H__