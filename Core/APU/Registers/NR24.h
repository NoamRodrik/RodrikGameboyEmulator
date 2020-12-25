/**
 * @file		NR24.h
 * @author		Noam Rodrik
 * @description LR35902's main apu NR24 definition header.
 */
#ifndef __LR35902_NR24_H__
#define __LR35902_NR24_H__

#include <Core/CPU/Registers/DeviceRegister.h>

namespace Core
{
class [[nodiscard]] NR24 : public DeviceRegister<0xFF19>
{
public:
	using DeviceRegister::DeviceRegister;
	using DeviceRegister::operator=;
	using DeviceRegister::operator API::data_t;

public:
	[[nodiscard]] const bool IsLengthExpired() const
	{
		return Tools::IsBitSet(static_cast<API::data_t>(*this), NR24_LENGTH_EXPIRES_BIT);
	}

public:
	static constexpr API::data_t NR24_RESTART_BIT{0x07};
	static constexpr API::data_t NR24_LENGTH_EXPIRES_BIT{0x06};
	static constexpr API::data_t NR24_DEFAULT_VALUE{0xBF};
};
} // Core

#endif // __LR35902_NR24_H__