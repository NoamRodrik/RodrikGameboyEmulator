/**
 * @file		SoundChannel3.cpp
 * @author		Noam Rodrik
 * @description Main logic of the APU's sound channel 3.
 */
#include <Core/APU/Waves/Implementation/SoundChannel3.h>
#include <Core/APU/Registers/NR34.h>
#include <Core/APU/Registers/NR32.h>
#include <Core/APU/Registers/NR31.h>
#include <Core/APU/Definitions.h>

namespace Core
{
void SoundChannel3::Restart()
{
	this->SetEnabled(true);
	this->UpdateStatus();

	// NOTE definitions
	this->GetLength().SetLength(0x100 - static_cast<API::data_t>(NR31{}), NR34{}.IsLengthExpired());

	this->SetClocks(0x00);
	this->_sample_index = 0x00;
}

void SoundChannel3::LengthTick()
{
	this->SetEnabled(this->GetLength().Play());
	this->UpdateStatus();
}

void SoundChannel3::Clock(uint8_t clocks)
{
	const std::size_t CURRENT_SAMPLE_UPDATE_RATE = (2048 - static_cast<std::size_t>(this->GetFrequency() & 0b11111111111)) * 2;

	this->SetClocks(this->GetClocks() + clocks);
	if (this->GetClocks() >= CURRENT_SAMPLE_UPDATE_RATE)
	{
		this->_sample_index = (this->_sample_index == 31 ? 0 : this->_sample_index + 1);
		this->UpdateSample();
		this->SetClocks(this->GetClocks() - CURRENT_SAMPLE_UPDATE_RATE);
	}
}

void SoundChannel3::Start()
{
	if (!this->_started)
	{
		this->_started = true;
		this->Restart();
	}
}

void SoundChannel3::Stop()
{
	this->_started = false;
	this->SetEnabled(false);
}

void SoundChannel3::UpdateSample()
{
	// Each byte in the Wave RAM holds two 2 4-bit samples
	API::data_t given_sample{0x00};
	SANITY(Processor::GetInstance().GetMemory().Read(API::WAVE_PATTERN_RAM_OFFSET + (this->_sample_index / 2), given_sample),
		"Failed reading wave ram!");
	this->SetCurrentSample(this->_sample_index % 2 == 0 ? (given_sample & 0xF0) >> 4 : given_sample & 0x0F);

	const auto output_volume_level = NR32{}.GetOutputLevel();

	if (output_volume_level != NR32::OutputLevel::MUTED)
	{
		this->SetCurrentSample(this->GetCurrentSample() >> (static_cast<API::data_t>(output_volume_level) - 1));
	}

	if (!this->IsEnabled() || !this->_started || output_volume_level == NR32::OutputLevel::MUTED)
	{
		this->SetCurrentSample(0x00);
	}
}
} // Core