/**
 * @file		ASoundChannel.h
 * @author		Noam Rodrik
 * @description An abstract interface for any sound channel
 */
#ifndef __CORE_ASOUND_CHANNEL_H__
#define __CORE_ASOUND_CHANNEL_H__

#include <Core/APU/Waves/ISoundChannel.h>
#include <Core/APU/Registers/NR51.h>
#include <Core/APU/Registers/NR52.h>
#include <API/APU/Effects/Length.h>
#include <Core/APU/Definitions.h>
#include <API/Definitions.h>
#include <cstddef>
#include <atomic>

namespace Core
{
template <SoundChannel SOUND_CHANNEL>
class [[nodiscard]] ASoundChannel : public ISoundChannel
{
public:
	virtual ~ASoundChannel() override = default;

public:
	[[nodiscard]] virtual const bool IsEnabled() const override
	{
		return this->_enabled;
	}

	virtual void SetEnabled(bool status) override
	{
		this->_enabled = status;
	}

	[[nodiscard]] virtual const bool Activated(const OutputTerminal output) const override
	{
		return (NR51{} >> (NR51::OutputTerminalOffset(output) + (static_cast<size_t>(SOUND_CHANNEL) - 1))) & 0b00000001;
	}

	virtual void SetLength(const API::Length& new_length) override
	{
		this->_length = new_length;
	}

	[[nodiscard]] virtual const API::Length& GetLength() const override
	{
		return this->_length;
	}

	[[nodiscard]] virtual API::Length& GetLength() override
	{
		return this->_length;
	}

	virtual void SetCurrentSample(const API::data_t current_sample) override
	{
		this->_current_sample = current_sample;
	}

	[[nodiscard]] virtual const API::data_t GetCurrentSample() const override
	{
		return this->_current_sample;
	}

	virtual void SetClocks(const std::size_t clocks) override
	{
		this->_clocks = clocks;
	}

	[[nodiscard]] virtual const std::size_t GetClocks() const override
	{
		return this->_clocks;
	}

	virtual void SetSequence(const API::data_t sequence) override
	{
		this->_sequence = sequence;
	}

	virtual const API::data_t GetSequence() const override
	{
		return this->_sequence;
	}

	virtual void SetFrequency(const API::address_t frequency) override
	{
		this->_frequency = frequency;
	}

	[[nodiscard]] virtual const API::address_t GetFrequency() const override
	{
		return this->_frequency;
	}

	/**
	 * Default implementation
	 */
	virtual void UpdateSample() override {}
	virtual void EnvelopeTick() override {}
	virtual void SweepTick() override {}

protected:
	void UpdateStatus() const
	{
		NR52 nr52{};
		nr52.SetStatus(SOUND_CHANNEL, this->_enabled);
	}

protected:
	static constexpr SoundChannel MY_CHANNEL{SOUND_CHANNEL};

protected:
	std::atomic<API::data_t>    _sequence{0x00};
	std::atomic<API::address_t> _frequency{0x00};

private:
	API::Length       _length{};
	std::atomic<bool> _enabled{false};
	std::size_t       _clocks{0x00};
	API::data_t       _current_sample{0x00};
};
} // Core

#endif // __CORE_ASOUND_CHANNEL_H__