/**
 * @file		SoundChannel1.h
 * @author		Noam Rodrik
 * @description The implementation for LR35902's sound channel 1 in the APU.
 */
#ifndef __CORE_SOUND_CHANNEL_1_H__
#define __CORE_SOUND_CHANNEL_1_H__

#include <Core/APU/Waves/Implementation/ASoundChannelSquare.h>
#include <Core/APU/Definitions.h>
#include <API/Definitions.h>
#include <cstddef>

namespace Core
{
class [[nodiscard]] SoundChannel1 : public ASoundChannelSquare<SoundChannel::PULSE_A>
{
public:
	SoundChannel1() = default;
	virtual ~SoundChannel1() override = default;

public:
	virtual void Restart() override;
	virtual void UpdateSample() override;

	/* Clock functions */
	virtual void SweepTick() override;

private:
	uint8_t _sweep_time_elapsed{0x00};
};
} // Core

#endif // __CORE_SOUND_CHANNEL_1_H__