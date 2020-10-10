/**
 * @file   Envelope.h
 * @author Noam Rodrik
 * @description Class that helps encapsulate envelope data and operations.
 */
#ifndef __API_ENVELOPE_H__
#define __API_ENVELOPE_H__

#include <API/APU/Effects/IEffect.h>
#include <stdint.h>

namespace API
{
class [[nodiscard]] Envelope : public IEffect
{
public:
	Envelope() = default;
	virtual ~Envelope() override = default;
	Envelope(uint8_t length, uint8_t volume, bool direction) :
		_length{length},
		_volume{volume},
		_direction{direction} {}

public:
	virtual bool Play() override
	{
		if (this->_length != 0)
		{
			if (this->_length != this->_elapsed_time)
			{
				this->_elapsed_time += 1;
			}
			else
			{
				// If the envelope isn't silent yet upon decreasing it's volume,
				// we want to keep decreasing.
				if (this->IsDecreasing() && !this->IsSilent())
				{
					this->_volume -= 1;
				}
				// If the envelope isn't on maximum volume yet upon increasing it's volume,
				// we want to keep increasing.
				else if (this->IsIncreasing() && !this->IsVolumeOnMax())
				{
					this->_volume += 1;
				}

				// Reset elapsed time.
				this->_elapsed_time = 0x00;
			}
		}

		return true;
	}

	[[nodiscard]] const uint8_t GetVolume() const
	{
		return this->_volume;
	}

private:
	[[nodiscard]] const bool IsIncreasing() const
	{
		return this->_direction;
	}

	[[nodiscard]] const bool IsDecreasing() const
	{
		return !this->_direction;
	}

	[[nodiscard]] const bool IsSilent() const
	{
		return this->_volume == 0x00;
	}

	[[nodiscard]] const bool IsVolumeOnMax() const
	{
		return this->_volume == 0x0F;
	}

private:
	uint8_t _elapsed_time{0x00};
	uint8_t _length{0x00};
	/* Range between 0x00 - 0x0F */
	uint8_t _volume{0x00};
	bool    _direction{false};
};
} // API

#endif // __API_ENVELOPE_H__