/**
 * @file		ArithmeticRegisters.h
 * @author		Noam Rodrik
 * @description LR35902 general registers definition header.
 */
#ifndef __LR35902_ARITHMETIC_REGISTERS_H__
#define __LR35902_ARITHMETIC_REGISTERS_H__

#include <Core/Registers/RegisterBase.h>

namespace Core
{
class ArithmeticRegister : public ArithmeticRegisterBase
{
public:
	using ArithmeticRegisterBase::ArithmeticRegisterBase;
	using ArithmeticRegisterBase::RegisterType;
	using ArithmeticRegisterBase::operator ArithmeticRegisterBase::RegisterType&;

	ArithmeticRegister& operator>>=(int amount)
	{
		this->m_data >>= amount;
		return *this;
	}

	ArithmeticRegister& operator<<=(int amount)
	{
		this->m_data <<= amount;
		return *this;
	}

	ArithmeticRegister& operator&=(data_t data)
	{
		this->m_data &= data;
		return *this;
	}

	ArithmeticRegister& operator|=(data_t data)
	{
		this->m_data |= data;
		return *this;
	}

	auto operator ~()
	{
		return ~this->m_data;
	}

	void operator<<=(uint8_t) const = delete;
	void operator&=(uint8_t) const = delete;
	void operator|=(uint8_t) const = delete;
	void operator>>=(uint8_t) const = delete;
};

static_assert(sizeof(ArithmeticRegister) == sizeof(ArithmeticRegister::RegisterType));
} // Core

#endif // __LR35902_ARITHMETIC_REGISTERS_H__
