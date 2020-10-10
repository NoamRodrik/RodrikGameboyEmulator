/**
 * @file		EFLRegisters.h
 * @author		Noam Rodrik
 * @description LR35902 main EFL registers definition header.
 */
#ifndef __LR35902_EFL_REGISTERS_H__
#define __LR35902_EFL_REGISTERS_H__

#include <API/Registers/RegisterBase.h>
#include <API/Definitions.h>

namespace Core
{
/* Flag Register Index */
enum class [[nodiscard]] Flag : API::data_t
{
	ZERO = 7,
	SUB = 6,
	HALF_CARRY = 5,
	CARRY = 4
};

static_assert(static_cast<uint8_t>(Flag::ZERO) < CHAR_BIT &&
			  static_cast<uint8_t>(Flag::SUB) < CHAR_BIT &&
	          static_cast<uint8_t>(Flag::HALF_CARRY) < CHAR_BIT &&
	          static_cast<uint8_t>(Flag::CARRY) < CHAR_BIT);

class [[nodiscard]] EFL_Register : public API::FlagsRegisterBase
{
public:
	using API::FlagsRegisterBase::FlagsRegisterBase;
	using API::FlagsRegisterBase::RegisterType;

public:
	[[nodiscard]] constexpr bool IsSet(const Flag flag) const
	{
		return API::FlagsRegisterBase::GetFlag(static_cast<const uint8_t>(flag));
	}

	inline void Set(const Flag flag)
	{
		API::FlagsRegisterBase::SetFlag(static_cast<const uint8_t>(flag));
	}

	inline void Clear(const Flag flag)
	{
		API::FlagsRegisterBase::ClearFlag(static_cast<const uint8_t>(flag));
	}

	void MutateByCondition(const bool condition, const Flag flag)
	{
		if (condition)
		{
			this->Set(flag);
		}
		else
		{
			this->Clear(flag);
		}
	}
};

static_assert(sizeof(EFL_Register) == sizeof(EFL_Register::RegisterType));
} // Core

#endif // __LR35902_EFL_REGISTERS_H__
