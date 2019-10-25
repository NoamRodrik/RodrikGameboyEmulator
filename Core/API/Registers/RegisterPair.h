/**
 * @file		RegisterPair.h
 * @author		Roy Abudi
 * @description A wrapper class for the conjuction of 2 registers
 */
#ifndef __REGISTER_PAIR_H__
#define __REGISTER_PAIR_H__

#include <Core/API/Definitions.h>
#include <limits>

namespace Core
{
static_assert(std::numeric_limits<data_t>::digits == 8);
static_assert(std::numeric_limits<address_t>::digits == std::numeric_limits<data_t>::digits * 2);

class RegisterPair
{
public:
	constexpr RegisterPair(data_t& msb, data_t& lsb) : m_msb{ msb }, m_lsb{ lsb } {}

	constexpr RegisterPair() = delete;
	constexpr RegisterPair(const RegisterPair& other) = delete;
	~RegisterPair() = default;

	constexpr RegisterPair& operator=(const RegisterPair& other)
	{
		m_msb = other.m_msb;
		m_lsb = other.m_lsb;
		return *this;
	}
	constexpr RegisterPair& operator=(const address_t other)
	{
		m_msb = get_msb(other);
		m_lsb = get_lsb(other);
		return *this;
	}
	constexpr operator address_t() const
	{
		return (address_t{m_msb} << std::numeric_limits<data_t>::digits) | m_lsb;
	}

private:
	static constexpr data_t get_msb(const address_t value)
	{
		return (value >> std::numeric_limits<data_t>::digits) & ~data_t{0};
	}

	static constexpr data_t get_lsb(const address_t value)
	{
		return value & ~data_t{0};
	}

private:
	data_t& m_msb;
	data_t& m_lsb;
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
} // Core

#endif // __REGISTER_PAIR_H__