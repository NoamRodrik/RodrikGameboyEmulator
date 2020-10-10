/**
 * @file		NR24.h
 * @author		Noam Rodrik
 * @description LR35902's main apu NR24 definition header.
 */
#ifndef __LR35902_NR24_H__
#define __LR35902_NR24_H__

#include <Core/CPU/Registers/MemoryRegister.h>

namespace Core
{
class [[nodiscard]] NR24 : public MemoryRegister<API::data_t>
{
public:
	constexpr NR24() : MemoryRegister{NR24_ADDRESS} {}
	NR24(const API::data_t value) : MemoryRegister{NR24_ADDRESS} { *this = value; }

public:
	using MemoryRegister::operator=;
	using MemoryRegister::operator API::data_t;

public:
	[[nodiscard]] const bool IsLengthExpired() const
	{
		return Tools::IsBitSet(static_cast<API::data_t>(*this), NR24_LENGTH_EXPIRES_BIT);
	}

public:
	static constexpr API::data_t    NR24_RESTART_BIT{0x07};
	static constexpr API::data_t    NR24_LENGTH_EXPIRES_BIT{0x06};
	static constexpr API::address_t NR24_ADDRESS{0xFF19};
	static constexpr API::address_t NR24_DEFAULT_VALUE{0xBF};
};
} // Core

#endif // __LR35902_NR24_H__