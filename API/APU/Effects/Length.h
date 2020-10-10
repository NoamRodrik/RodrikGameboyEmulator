/**
 * @file   Length.h
 * @author Noam Rodrik
 * @description Class that helps encapsulate length data and operations.
 */
#ifndef __API_LENGTH_H__
#define __API_LENGTH_H__

#include <API/APU/Effects/IEffect.h>
#include <stdint.h>

namespace API
{
class [[nodiscard]] Length : public IEffect
{
public:
	Length() = default;
	Length(uint16_t length, bool terminate) :
		_length{length},
		_terminate{terminate} {}

	virtual ~Length() override = default;

public:
	void SetLength(uint16_t new_length, bool new_terminate)
	{
		// Keeping it's value until it reaches zero.
		this->_length = this->_length == 0 ? new_length : this->_length;
		this->_terminate = new_terminate;
	}

	[[nodiscard]] virtual bool Play() override
	{
		// Decreasing length only if it hasn't reached zero (not to underflow)
		this->_length -= this->_length != 0 ? 1 : 0;
		return !this->_terminate || (this->_length != 0 && this->_terminate);
	}

private:
	uint16_t _length{0x00};
	bool     _terminate{false};
};
} // API

#endif // __API_LENGTH_H__