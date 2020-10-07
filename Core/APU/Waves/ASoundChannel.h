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
#include <Core/APU/Definitions.h>
#include <API/Definitions.h>
#include <cstddef>
#include <atomic>

namespace Core
{
template <SoundChannel SOUND_CHANNEL>
class ASoundChannel : public ISoundChannel
{
public:
	virtual ~ASoundChannel() override = default;

public:
	virtual const bool IsEnabled() const override
	{
		return this->_enabled;
	}

	virtual void Enable() override
	{
		this->_enabled = true;
		this->UpdateStatus();
	}

	virtual void Disable() override
	{
		this->_enabled = false;
		this->UpdateStatus();
	}

	virtual const bool Activated(const OutputTerminal output) const override
	{
		return (NR51{} >> (NR51::OutputTerminalOffset(output) + (static_cast<size_t>(SOUND_CHANNEL) - 1))) & 0b00000001;
	}

private:
	void UpdateStatus() const
	{
		NR52 nr52{};
		nr52.SetStatus(SOUND_CHANNEL, this->_enabled);
	}

protected:
	static constexpr SoundChannel MY_CHANNEL{SOUND_CHANNEL};

private:
	std::atomic<bool> _enabled{false};
};
} // Core

#endif // __CORE_ASOUND_CHANNEL_H__