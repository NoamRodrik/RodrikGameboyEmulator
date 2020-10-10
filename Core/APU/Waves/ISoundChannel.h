/**
 * @file		ISoundChannel.h
 * @author		Noam Rodrik
 * @description An interface for any sound channel
 */
#ifndef __CORE_ISOUND_CHANNEL_H__
#define __CORE_ISOUND_CHANNEL_H__

#include <Core/APU/Definitions.h>
#include <API/Definitions.h>
#include <cstddef>

namespace API
{
class Length;
}

namespace Core
{
class [[nodiscard]] ISoundChannel
{
public:
	virtual ~ISoundChannel() = default;

public:
	virtual void UpdateSample() = 0;
	[[nodiscard]] virtual const float_t Sample(const float_t time) const = 0;
	virtual void Cycle(const uint8_t cycles) = 0;
	virtual void Restart() = 0;

	/* Clock functions */
	virtual void LengthTick() = 0;
	virtual void EnvelopeTick() = 0;
	virtual void SweepTick() = 0;

	/* Setters and Getters */
	virtual void SetLength(const API::Length& new_length) = 0;
	[[nodiscard]] virtual const API::Length& GetLength() const = 0;
	[[nodiscard]] virtual API::Length& GetLength() = 0;
	virtual void SetCurrentSample(const API::data_t current_sample) = 0;
	[[nodiscard]] virtual const API::data_t GetCurrentSample() const = 0;
	virtual void SetCycles(const std::size_t cycles) = 0;
	[[nodiscard]] virtual const std::size_t GetCycles() const = 0;
	virtual void SetSequence(const API::data_t sequence) = 0;
	[[nodiscard]] virtual const API::data_t GetSequence() const = 0;
	virtual void SetFrequency(const API::address_t frequency) = 0;
	[[nodiscard]] virtual const API::address_t GetFrequency() const = 0;

	[[nodiscard]] virtual const bool IsEnabled() const = 0;
	virtual void SetEnabled(bool status) = 0;

	[[nodiscard]] virtual const bool Activated(const OutputTerminal output) const = 0;
};
} // Core

#endif // __CORE_ISOUND_CHANNEL_H__