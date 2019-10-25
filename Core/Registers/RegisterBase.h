/**
 * @file		RegisterBase.h
 * @author		Noam Rodrik
 * @description Class definitions for register types
 */
#ifndef __REGISTER_BASE_H__
#define __REGISTER_BASE_H__

#include <Core/API/Definitions.h>
#include <type_traits>
#include <stdint.h>
#include <atomic>

namespace Core
{
/**
 * A base class made for registers.
 */
template <typename T>
class RegisterBase
{
public:
	static_assert(std::is_integral<T>::value, "Got a non-integral register-base data type.");

public:
	constexpr RegisterBase(T&& data) : m_data{data} {};

	operator T&()
	{
		return this->m_data;
	}

	operator T() const
	{
		return this->m_data;
	}

protected:
	using RegisterType = T;
	Message("Restore atomics if needed!");
	T m_data;
};

/**
 * All addressing registers use 16 bits.
 */
class AddressRegisterBase : public RegisterBase<address_t>
{
public:
	using RegisterBase::RegisterBase;
	using RegisterBase::operator address_t&;
	using RegisterBase::operator address_t;


	void operator=(const address_t data)
	{
		this->m_data = data;
	}
};

/**
 * In use for stack arithmetic
 */
class StackRegisterBase : public AddressRegisterBase
{
public:
	using AddressRegisterBase::AddressRegisterBase;
	using RegisterBase::operator address_t&;
	using RegisterBase::operator address_t;

	void operator=(const address_t data)
	{
		this->m_data = data;
	}

public:
	virtual void Push(const address_t data) = 0;
	virtual void Push(const data_t data) = 0;
	virtual void Pop(address_t& data) = 0;
	virtual void Pop(data_t& data) = 0;
};

/**
 * The stack registers hold 8 bit data segments that maintain their position in the stack.
 */
class FlagsRegisterBase : public RegisterBase<data_t>
{
public:
	using RegisterBase::RegisterBase;
	using RegisterBase::operator data_t&;
	using RegisterBase::operator data_t;

protected:
	constexpr bool GetFlag(const data_t bit_index) const
	{
		return this->GetFlag(this->m_data & (0b00000001 << bit_index), bit_index);
	}

	inline void SetFlag(const data_t flag)
	{
		this->m_data |= flag;
	}

	inline void ClearFlag(const data_t flag)
	{
		this->m_data &= (flag ^ 0b11111111);
	}

private:
	static constexpr bool GetFlag(const data_t flag, const data_t bit)
	{
		return (bit >> flag) & 0b00000001;
	}
};

/**
 * The arithmetic registers hold 8 bit data segments, that maintain their logic.
 */
class ArithmeticRegisterBase : public RegisterBase<data_t>
{
public:
	using RegisterBase::RegisterBase;
	using RegisterBase::operator data_t&;
	using RegisterBase::operator data_t;
};
} // Core

#endif // __REGISTER_BASE_H__