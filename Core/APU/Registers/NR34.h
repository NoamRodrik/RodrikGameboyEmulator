/**
 * @file		NR34.h
 * @author		Noam Rodrik
 * @description LR35902's main apu NR34 definition header.
 */
#ifndef __LR35902_NR34_H__
#define __LR35902_NR34_H__

#include <Core/CPU/Registers/DeviceRegister.h>

namespace Core
{
class [[nodiscard]] NR34 : public DeviceRegister<0xFF1E>
{
public:
	using DeviceRegister::DeviceRegister;
	using DeviceRegister::operator=;
	using DeviceRegister::operator API::data_t;

public:
	[[nodiscard]] const bool IsLengthExpired() const
	{
		return Tools::IsBitSet(static_cast<API::data_t>(*this), NR34_LENGTH_EXPIRES_BIT);
	}

public:
	static constexpr API::data_t NR34_RESTART_BIT{0x07};
	static constexpr API::data_t NR34_LENGTH_EXPIRES_BIT{0x06};
	static constexpr API::data_t NR34_DEFAULT_VALUE{0xBF};
};
} // Core

#endif // __LR35902_NR34_H__