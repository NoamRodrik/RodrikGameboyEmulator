/**
 * @file		SoundChannel1.cpp
 * @author		Noam Rodrik
 * @description Main logic of the APU's sound channel 1.
 */
#include <Core/APU/Waves/Implementation/SoundChannel1.h>
#include <Core/APU/Registers/NR51.h>
#include <Core/APU/Registers/NR14.h>
#include <Core/APU/Registers/NR13.h>
#include <Core/APU/Registers/NR12.h>
#include <Core/APU/Registers/NR11.h>
#include <Core/APU/Registers/NR10.h>
#include <Core/APU/Definitions.h>
#include <Core/CPU/Processor.h>

using namespace API;

namespace Core
{
const float_t SoundChannel1::Sample(const float_t time) const
{
	// Square wave
	Message("TODO");
	return 0;
}

void SoundChannel1::Restart()
{
	this->SetEnabled(true);

	// NOTE definitions
	this->GetLength().SetLength(AUDIO_DEFAULT_NOTE_LENGTH - NR11{}.GetLength(), NR14{}.IsLengthExpired());

	// ENVELOPE definitions
	const NR12 nr12{};
	this->_envelope = Envelope{nr12.GetEnvelopeSweep(), nr12.GetEnvelopeVolume(), nr12.GetEnvelopeDirection()};

	// Normal definitions
	this->SetCycles(0x00);
	this->_sample_index = 0x00;

	// ONLY SoundChannel1 has sweep!
	this->_sweep_time_elapsed = 0x00;
}

void SoundChannel1::Cycle(const uint8_t cycles)
{
	// No magic number here, this is how the PANDOCS defines the sample rate.
	const std::size_t CURRENT_SAMPLE_UPDATE_RATE = (2048 - static_cast<std::size_t>(this->GetFrequency() & 0b11111111111)) * 4;
	this->SetCycles(this->GetCycles() + cycles);

	if (this->GetCycles() >= CURRENT_SAMPLE_UPDATE_RATE)
	{
		this->_sample_index = (this->_sample_index == 7 ? 0 : this->_sample_index + 1);
		this->UpdateSample();
		this->SetCycles(this->GetCycles() - CURRENT_SAMPLE_UPDATE_RATE);
	}
}

void SoundChannel1::UpdateSample()
{
	this->SetCurrentSample(this->IsEnabled() ? SQUARE_WAVES_TYPES[(NR11{} &0xC0) >> NR11::SEQUENCE_BIT][this->_sample_index] * this->_envelope.GetVolume() : 0x00);
}

void SoundChannel1::LengthTick()
{
	this->SetEnabled(this->GetLength().Play());
}

void SoundChannel1::EnvelopeTick()
{
	SANITY(this->_envelope.Play(), "Failed playing envelope on note");
}

void SoundChannel1::SweepTick()
{
	const NR10 nr10{};

	// We can't play sweep if it isn't on.
	if (nr10.GetSweepTime() != 0)
	{
		if (this->_sweep_time_elapsed != nr10.GetSweepTime())
		{
			this->_sweep_time_elapsed += 1;
		}
		else
		{
			// This will cause a possible overflow/underflow, but this is how the APU works.
			const uint8_t NEW_SWEEP = (this->GetFrequency() >> nr10.GetSweepShiftAmount()) * (nr10.IsSweepDecreasing() ? -1 : 1);

			// If we underflowed on decrease, that means the sweep has finished.
			if (nr10.IsSweepDecreasing() && NEW_SWEEP > this->GetFrequency())
			{
				this->_sweep_time_elapsed = 0;
			}
			// If we overflowed on increase => Stop SoundChannel1 from playing.
			else if (nr10.IsSweepIncreasing() && (NEW_SWEEP + this->GetFrequency()) > 2047)
			{
				this->SetEnabled(false);
			}
			else
			{
				// We can change the frequency here.
				this->SetFrequency(this->GetFrequency() + NEW_SWEEP);
				this->SetCycles(0x00);
				this->_sweep_time_elapsed = 0x00;

				// Set frequency in memory
				SANITY(Processor::GetInstance().GetMemory().WriteDirectly(NR13::NR13_ADDRESS, this->GetFrequency() & 0b11111111),
					   "Failed writing to NR13");
				SANITY(Processor::GetInstance().GetMemory().WriteDirectly(NR14::NR14_ADDRESS, (NR14{} & 0xF8) | ((this->GetFrequency() >> CHAR_BIT) & 0b111)),
					"Failed writing to NR13");
			}
		}
	}
}
} // Core