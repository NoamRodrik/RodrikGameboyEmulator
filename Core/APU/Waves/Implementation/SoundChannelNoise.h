/**
 * @file		SoundChannelNoise.h
 * @author		Noam Rodrik
 * @description The implementation for LR35902's sound channel noise in the APU.
 */
#ifndef __CORE_SOUND_CHANNEL_NOISE_H__
#define __CORE_SOUND_CHANNEL_NOISE_H__

#include <Core/APU/Waves/ASoundChannel.h>
#include <Core/APU/Definitions.h>
#include <API/Definitions.h>
#include <cstddef>

namespace Core
{
class SoundChannelNoise : public ASoundChannel<SoundChannel::NOISE>
{
public:
	SoundChannelNoise() = default;
	virtual ~SoundChannelNoise() override = default;
};
} // Core

#endif // __CORE_SOUND_CHANNEL_NOISE_H__