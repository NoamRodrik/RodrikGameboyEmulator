/**
 * @file		SoundChannel2.h
 * @author		Noam Rodrik
 * @description The implementation for LR35902's sound channel 2 in the APU.
 */
#ifndef __CORE_SOUND_CHANNEL_2_H__
#define __CORE_SOUND_CHANNEL_2_H__

#include <Core/APU/Waves/ASoundChannel.h>
#include <Core/APU/Definitions.h>
#include <API/Definitions.h>
#include <cstddef>

namespace Core
{
class [[nodiscard]] SoundChannel2 : public ASoundChannel<SoundChannel::PULSE_B>
{
public:
	SoundChannel2() = default;
	virtual ~SoundChannel2() override = default;
};
} // Core

#endif // __CORE_SOUND_CHANNEL_2_H__