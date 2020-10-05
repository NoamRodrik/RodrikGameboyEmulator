/**
 * @file		RegisterPair.h
 * @author		Roy Abudi
 * @description A wrapper class for the conjuction of 2 registers
 */
#ifndef __REGISTER_PAIR_H__
#define __REGISTER_PAIR_H__

#include <API/Definitions.h>
#include <limits>

namespace API
{
static_assert(std::numeric_limits<data_t>::digits == 8);
static_assert(std::numeric_limits<address_t>::digits == std::numeric_limits<data_t>::digits * 2);

class RegisterPair
{
public:
	constexpr RegisterPair(data_t& msb, data_t& lsb) : _msb{msb}, _lsb{lsb} {}

	constexpr RegisterPair() = delete;
	constexpr RegisterPair(const RegisterPair& other) = delete;
	~RegisterPair() = default;

	constexpr RegisterPair& operator=(const RegisterPair& other)
	{
		this->_msb = other._msb;
		this->_lsb = other._lsb;
		return *this;
	}

	constexpr RegisterPair& operator=(const address_t other)
	{
		_msb = get_msb(other);
		_lsb = get_lsb(other);
		return *this;
	}

	constexpr operator address_t() const
	{
		return ((address_t{this->_msb} << std::numeric_limits<data_t>::digits) & 0xFF00) | (this->_lsb & 0x00FF);
	}

private:
	static constexpr data_t get_msb(const address_t value)
	{
		return (value >> std::numeric_limits<data_t>::digits) & 0x00FF;
	}

	static constexpr data_t get_lsb(const address_t value)
	{
		return value & 0x00FF;
	}

private:
	data_t& _msb;
	data_t& _lsb;
};

constexpr RegisterPair& operator+=(RegisterPair& reg_pair, const address_t value)
{
	return reg_pair = reg_pair + value;
}
constexpr RegisterPair& operator-=(RegisterPair& reg_pair, const address_t value)
{
	return reg_pair = reg_pair - value;
}
constexpr RegisterPair& operator*=(RegisterPair& reg_pair, const address_t value)
{
	return reg_pair = reg_pair * value;
}
constexpr RegisterPair& operator/=(RegisterPair& reg_pair, const address_t value)
{
	SANITY(value != 0, "Can't divide by zero!");
	return reg_pair = reg_pair / value;
}
constexpr RegisterPair& operator++(RegisterPair& reg_pair)
{
	return reg_pair += 1;
}
} // API

#endif // __REGISTER_PAIR_H__