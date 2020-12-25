/**
 * @file		NR41.h
 * @author		Noam Rodrik
 * @description LR35902's main apu NR41 definition header.
 */
#ifndef __LR35902_NR41_H__
#define __LR35902_NR41_H__

#include <Core/CPU/Registers/DeviceRegister.h>

namespace Core
{
class [[nodiscard]] NR41 : public DeviceRegister<0xFF20>
{
public:
	using DeviceRegister::DeviceRegister;
	using DeviceRegister::operator=;
	using DeviceRegister::operator API::data_t;

public:
	[[nodiscard]] const API::data_t GetLength() const
	{
		return (static_cast<API::data_t>(*this) >> NR41_LENGTH_BIT) & 0b111111;
	}

public:
	static constexpr API::data_t NR41_LENGTH_BIT{0x00};
	static constexpr API::data_t NR41_DEFAULT_VALUE{0xFF};
};
} // Core

#endif // __LR35902_NR41_H__