/**
 * @file		NR30.h
 * @author		Noam Rodrik
 * @description LR35902's main apu NR30 definition header.
 */
#ifndef __LR35902_NR30_H__
#define __LR35902_NR30_H__

#include <Core/CPU/Registers/DeviceRegister.h>

namespace Core
{
class [[nodiscard]] NR30 : public DeviceRegister<0xFF1A>
{
public:
	using DeviceRegister::DeviceRegister;
	using DeviceRegister::operator=;
	using DeviceRegister::operator API::data_t;

public:
	[[nodiscard]] const bool IsSoundOn() const
	{
		return (static_cast<API::data_t>(*this) >> NR30_ACTIVATED_BIT) & 0b01;
	}

public:
	static constexpr API::data_t NR30_ACTIVATED_BIT{0x07};
	static constexpr API::data_t NR30_DEFAULT_VALUE{0x7F};
};
} // Core

#endif // __LR35902_NR30_H__