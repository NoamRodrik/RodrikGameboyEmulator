/** 
 * @file   ASoundChannelSquare.h
 * @author Noam Rodrik
 * @description A unifying class for all square-wave like channels.
 */
#ifndef __CORE_ASOUND_CHANNEL_SQUARE_H__
#define __CORE_ASOUND_CHANNEL_SQUARE_H__

#include <Core/APU/Waves/ASoundChannel.h>
#include <API/APU/Effects/Envelope.h>
#include <Core/APU/Definitions.h>
#include <API/Definitions.h>

namespace Core
{
template <SoundChannel SOUND_CHANNEL>
class [[nodiscard]] ASoundChannelSquare : public ASoundChannel<SOUND_CHANNEL>
{
public:
	template <typename LENGTH_REGISTER, typename LENGTH_EXPIRED_REGISTER, typename ENVELOPE_REGISTER>
	void SquareChannelRestart()
	{
		this->SetEnabled(true);
		ASoundChannel::UpdateStatus();

		// NOTE definitions
		this->GetLength().SetLength(API::AUDIO_DEFAULT_NOTE_LENGTH - LENGTH_REGISTER{}.GetLength(), LENGTH_EXPIRED_REGISTER{}.IsLengthExpired());

		// ENVELOPE definitions
		const ENVELOPE_REGISTER envelope_register{};
		this->_envelope = API::Envelope{envelope_register.GetEnvelopeSweep(), envelope_register.GetEnvelopeVolume(), envelope_register.GetEnvelopeDirection()};

		// Normal definitions
		this->SetClocks(0x00);
		this->_sample_index = 0x00;
	}

	template <typename SAMPLE_REGISTER>
	void SquareChannelUpdateSamples()
	{
		this->SetCurrentSample(this->IsEnabled() ? SQUARE_WAVES_TYPES[(SAMPLE_REGISTER{} & 0xC0) >> typename SAMPLE_REGISTER::SEQUENCE_BIT][this->_sample_index] * this->_envelope.GetVolume() : 0x00);
	}

	virtual void Clock(const uint8_t clocks) override
	{
		// No magic number here, this is how the PANDOCS defines the sample rate.
		const std::size_t CURRENT_SAMPLE_UPDATE_RATE = (2048 - static_cast<std::size_t>(this->GetFrequency() & 0b11111111111)) * 4;
		this->SetClocks(this->GetClocks() + clocks);

		if (this->GetClocks() >= CURRENT_SAMPLE_UPDATE_RATE)
		{
			this->_sample_index = (this->_sample_index == 7 ? 0 : this->_sample_index + 1);
			this->UpdateSample();
			this->SetClocks(this->GetClocks() - CURRENT_SAMPLE_UPDATE_RATE);
		}
	}

	/* Clock functions */
	virtual void LengthTick() override
	{
		this->SetEnabled(this->GetLength().Play());
		ASoundChannel::UpdateStatus();
	}

	virtual void EnvelopeTick() override
	{
		SANITY(this->_envelope.Play(), "Failed playing envelope on note");
	}

protected:
	API::Envelope _envelope{};
	uint16_t	  _sample_index{0x00};
};
} // Core

#endif // __CORE_ASOUND_CHANNEL_SQUARE_H__