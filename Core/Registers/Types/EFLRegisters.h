/**
 * @file		EFLRegisters.h
 * @author		Noam Rodrik
 * @description LR35902 main EFL registers definition header.
 */
#ifndef __LR35902_EFL_REGISTERS_H__
#define __LR35902_EFL_REGISTERS_H__

#include <Core/Registers/RegisterBase.h>
#include <API/Definitions.h>

namespace Core
{
class EFL_Register : public FlagsRegisterBase
{
public:
	using FlagsRegisterBase::FlagsRegisterBase;
	using FlagsRegisterBase::RegisterType;

public:
	constexpr bool IsSet(const API::Flag flag) const
	{
		return FlagsRegisterBase::GetFlag(static_cast<const uint8_t>(flag));
	}

	inline void Set(const API::Flag flag)
	{
		FlagsRegisterBase::SetFlag(static_cast<const uint8_t>(flag));
	}

	inline void Clear(const API::Flag flag)
	{
		FlagsRegisterBase::ClearFlag(static_cast<const uint8_t>(flag));
	}

	void MutateByCondition(const bool condition, const API::Flag flag)
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
