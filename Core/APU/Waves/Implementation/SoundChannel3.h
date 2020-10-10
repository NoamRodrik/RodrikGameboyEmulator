/**
 * @file		SoundChannel3.h
 * @author		Noam Rodrik
 * @description The implementation for LR35902's sound channel 3 in the APU.
 */
#ifndef __CORE_SOUND_CHANNEL_3_H__
#define __CORE_SOUND_CHANNEL_3_H__

#include <Core/APU/Waves/ASoundChannel.h>
#include <Core/APU/Definitions.h>
#include <API/Definitions.h>
#include <cstddef>

namespace Core
{
class [[nodiscard]] SoundChannel3 : public ASoundChannel<SoundChannel::WAVE>
{
public:
	SoundChannel3() = default;
	virtual ~SoundChannel3() override = default;

public:
	virtual void UpdateSample() override;
	virtual void Restart() override;
	virtual void LengthTick() override;
	virtual void Clock(uint8_t clocks) override;
	void Start();
	void Stop();

private:
	uint8_t  _sample_index{0x00};
	bool     _started{false};
};
} // Core

#endif // __CORE_SOUND_CHANNEL_3_H__