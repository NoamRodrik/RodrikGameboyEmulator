/**
 * @file		SoundChannel2.h
 * @author		Noam Rodrik
 * @description The implementation for LR35902's sound channel 2 in the APU.
 */
#ifndef __CORE_SOUND_CHANNEL_2_H__
#define __CORE_SOUND_CHANNEL_2_H__

#include <Core/APU/Waves/Implementation/ASoundChannelSquare.h>
#include <Core/APU/Definitions.h>
#include <API/Definitions.h>
#include <cstddef>

namespace Core
{
class [[nodiscard]] SoundChannel2 : public ASoundChannelSquare<SoundChannel::PULSE_B>
{
public:
	SoundChannel2() = default;
	virtual ~SoundChannel2() override = default;

public:
	virtual void Restart() override;
	virtual void UpdateSample() override;
};
} // Core

#endif // __CORE_SOUND_CHANNEL_2_H__