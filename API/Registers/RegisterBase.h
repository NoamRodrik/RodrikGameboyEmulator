/**
 * @file		RegisterBase.h
 * @author		Noam Rodrik
 * @description Class definitions for register types
 */
#ifndef __REGISTER_BASE_H__
#define __REGISTER_BASE_H__

#include <API/Definitions.h>
#include <type_traits>
#include <stdint.h>
#include <atomic>

namespace API
{
/**
 * A base class made for registers.
 */
template <typename T>
class [[nodiscard]] RegisterBase
{
public:
	static_assert(std::is_integral<T>::value, "Got a non-integral register-base data type.");

public:
	constexpr explicit RegisterBase(T&& data) : _data{std::move(data)} {}

	constexpr operator T&()
	{
		return this->_data;
	}

	constexpr operator T() const
	{
		return this->_data;
	}

protected:
	using RegisterType = T;

protected:
	T _data{};
};

/**
 * All addressing registers use 16 bits.
 */
class [[nodiscard]] AddressRegisterBase : public RegisterBase<address_t>
{
public:
	using RegisterBase::RegisterBase;
	using RegisterBase::operator address_t&;
	using RegisterBase::operator address_t;

	AddressRegisterBase& operator=(const address_t data)
	{
		this->_data = data;
		return *this;
	}
};

/**
 * In use for stack arithmetic
 */
class [[nodiscard]] StackRegisterBase : public AddressRegisterBase
{
public:
	using AddressRegisterBase::AddressRegisterBase;
	using AddressRegisterBase::operator=;
	using RegisterBase::operator address_t&;
	using RegisterBase::operator address_t;

public:
	virtual void Push(const address_t data) = 0;
	virtual void Push(const data_t data) = 0;
	virtual void Pop(address_t& data) = 0;
	virtual void Pop(data_t& data) = 0;
};

/**
 * The stack registers hold 8 bit data segments that maintain their position in the stack.
 */
class [[nodiscard]] FlagsRegisterBase : public RegisterBase<data_t>
{
public:
	using RegisterBase::RegisterBase;
	using RegisterBase::operator data_t&;
	using RegisterBase::operator data_t;

protected:
	[[nodiscard]] constexpr bool GetFlag(const data_t bit_index) const
	{
		return (this->_data >> bit_index) & 0x01;
	}

	inline void SetFlag(const data_t flag)
	{
		this->_data |= (0x1 << flag);
	}

	inline void ClearFlag(const data_t flag)
	{
		this->_data &= ((0x1 << flag) ^ 0b11111111);
	}
};

/**
 * The arithmetic registers hold 8 bit data segments, that maintain their logic.
 */
class [[nodiscard]] ArithmeticRegisterBase : public RegisterBase<data_t>
{
public:
	using RegisterBase::RegisterBase;
	using RegisterBase::operator data_t&;
	using RegisterBase::operator data_t;
};
} // API

#endif // __REGISTER_BASE_H__