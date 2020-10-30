/**
 * @file		NR11.h
 * @author		Noam Rodrik
 * @description LR35902's main apu NR11 definition header.
 */
#ifndef __LR35902_NR11_H__
#define __LR35902_NR11_H__

#include <Core/CPU/Registers/DeviceRegister.h>

namespace Core
{
class [[nodiscard]] NR11 : public DeviceRegister<0xFF11>
{
public:
	using DeviceRegister::DeviceRegister;
	using DeviceRegister::operator=;
	using DeviceRegister::operator API::data_t;

public:
	[[nodiscard]] const API::data_t GetLength() const
	{
		return (static_cast<API::data_t>(*this) >> NR11_LENGTH_BIT) & 0b111111;
	}

public:
	static constexpr API::data_t NR11_LENGTH_BIT{0x00};
	static constexpr API::data_t SEQUENCE_BIT{0x06};
	static constexpr API::data_t NR11_DEFAULT_VALUE{0xBF};
};
} // Core

#endif // __LR35902_NR11_H__