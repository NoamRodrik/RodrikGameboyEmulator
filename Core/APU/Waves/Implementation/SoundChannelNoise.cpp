/**
 * @file		SoundChannelNoise.cpp
 * @author		Noam Rodrik
 * @description Main logic of the APU's sound channel noise.
 */
#include <Core/APU/Waves/Implementation/SoundChannelNoise.h>
#include <Core/APU/Definitions.h>
#include <Core/APU/Registers/NR44.h>
#include <Core/APU/Registers/NR43.h>
#include <Core/APU/Registers/NR42.h>
#include <Core/APU/Registers/NR41.h>

namespace Core
{
void SoundChannelNoise::Restart()
{
	this->SetEnabled(true);
	this->UpdateStatus();

	// NOTE definitions
	this->GetLength().SetLength(0x100 - NR41{}.GetLength(), NR44{}.IsLengthExpired());

	// ENVELOPE definitions
	const NR42 envelope_register{};
	this->_envelope = API::Envelope{envelope_register.GetEnvelopeSweep(), envelope_register.GetEnvelopeVolume(), envelope_register.GetEnvelopeDirection()};

	this->_linear_shift_register = 0x7FFF;	
	this->SetClocks(0x00);
	this->UpdateSample();

}

void SoundChannelNoise::Clock(uint8_t clocks)
{
	// No magic number here, this is how the PANDOCS defines the sample rate.
	const NR43 nr43{};
	const API::data_t NOISE{0x00};
	const uint16_t CURRENT_SAMPLE_UPDATE_RATE = DIVISOR_ARRAY[nr43.GetRatioOfFrequencies()] << (nr43.GetShiftClockFrequency() & 0x0F);

	this->SetClocks(this->GetClocks() + clocks);

	if (this->GetClocks() >= CURRENT_SAMPLE_UPDATE_RATE)
	{
		this->SetClocks(this->GetClocks() - CURRENT_SAMPLE_UPDATE_RATE);
		this->UpdateSample();
	}
}

void SoundChannelNoise::LengthTick()
{
	this->SetEnabled(this->GetLength().Play());
	this->UpdateStatus();
}

void SoundChannelNoise::EnvelopeTick()
{
	SANITY(this->_envelope.Play(), "Failed playing envelope on note");
}

void SoundChannelNoise::UpdateSample()
{
	const uint8_t XORED_SHIFTS = (this->_linear_shift_register & 0x01) ^ ((this->_linear_shift_register & 0x02) >> 1);
	this->_linear_shift_register = (XORED_SHIFTS << 14) | (this->_linear_shift_register >> 1);

	if (NR43{}.IsCounterStep7Bits())
	{
		this->_linear_shift_register = (XORED_SHIFTS << 6) | (this->_linear_shift_register & 0x7FBF);
	}

	this->SetCurrentSample((this->_linear_shift_register & 0x1) == 0 ? 1 : 0);

	if (!this->IsEnabled())
	{
		this->SetCurrentSample(0x00);
	}
}
} // Core