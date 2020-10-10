/**
 * @file		SoundChannelNoise.h
 * @author		Noam Rodrik
 * @description The implementation for LR35902's sound channel noise in the APU.
 */
#ifndef __CORE_SOUND_CHANNEL_NOISE_H__
#define __CORE_SOUND_CHANNEL_NOISE_H__

#include <Core/APU/Waves/ASoundChannel.h>
#include <API/APU/Effects/Envelope.h>
#include <Core/APU/Definitions.h>
#include <API/Definitions.h>
#include <cstddef>

namespace Core
{
class [[nodiscard]] SoundChannelNoise : public ASoundChannel<SoundChannel::NOISE>
{
public:
	SoundChannelNoise() = default;
	virtual ~SoundChannelNoise() override = default;

public:
	virtual void UpdateSample() override;
	virtual void Restart() override;
	virtual void Clock(uint8_t clocks) override;
	virtual void LengthTick() override;
	virtual void EnvelopeTick() override;

private:
	uint16_t      _linear_shift_register{0x7FFF};
	API::Envelope _envelope{};
};
} // Core

#endif // __CORE_SOUND_CHANNEL_NOISE_H__