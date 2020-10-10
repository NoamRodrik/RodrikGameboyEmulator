/**
 * @file		SoundChannel1.h
 * @author		Noam Rodrik
 * @description The implementation for LR35902's sound channel 1 in the APU.
 */
#ifndef __CORE_SOUND_CHANNEL_1_H__
#define __CORE_SOUND_CHANNEL_1_H__

#include <Core/APU/Waves/ASoundChannel.h>
#include <API/APU/Effects/Envelope.h>
#include <Core/APU/Definitions.h>
#include <API/Definitions.h>
#include <cstddef>

namespace Core
{
class [[nodiscard]] SoundChannel1 : public ASoundChannel<SoundChannel::PULSE_A>
{
public:
	SoundChannel1() = default;
	virtual ~SoundChannel1() override = default;

public:
	[[nodiscard]] virtual const float_t Sample(const float_t time) const override;
	virtual void Restart() override;
	virtual void Cycle(const uint8_t cycles) override;
	virtual void UpdateSample() override;

	/* Clock functions */
	virtual void LengthTick() override;
	virtual void EnvelopeTick() override;
	virtual void SweepTick() override;

private:
	API::Envelope _envelope{};
	uint16_t	  _sample_index{0x00};
	uint8_t		  _sweep_time_elapsed{0x00};
};
} // Core

#endif // __CORE_SOUND_CHANNEL_1_H__