/**
 * @file		SoundChannel1.h
 * @author		Noam Rodrik
 * @description The implementation for LR35902's sound channel 1 in the APU.
 */
#ifndef __CORE_SOUND_CHANNEL_1_H__
#define __CORE_SOUND_CHANNEL_1_H__

#include <Core/APU/Waves/ASoundChannel.h>
#include <Core/APU/Definitions.h>
#include <API/Definitions.h>
#include <cstddef>

namespace Core
{
class SoundChannel1 : public ASoundChannel<SoundChannel::PULSE_A>
{
public:
	SoundChannel1() = default;
	virtual ~SoundChannel1() override = default;
};
} // Core

#endif // __CORE_SOUND_CHANNEL_1_H__