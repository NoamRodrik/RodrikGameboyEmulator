/**
 * @file		NR32.h
 * @author		Noam Rodrik
 * @description LR35902's main apu NR32 definition header.
 */
#ifndef __LR35902_NR32_H__
#define __LR35902_NR32_H__

#include <Core/CPU/Registers/MemoryRegister.h>

namespace Core
{
class NR32 : public MemoryRegister<API::data_t>
{
public:
	constexpr NR32() : MemoryRegister{NR32_ADDRESS} {}
	NR32(const API::data_t value) : MemoryRegister{NR32_ADDRESS} { *this = value; }

public:
	using MemoryRegister::operator=;
	using MemoryRegister::operator API::data_t;

public:
	static constexpr API::address_t NR32_ADDRESS{0xFF1C};
	static constexpr API::address_t NR32_DEFAULT_VALUE{0x9F};
};
} // Core

#endif // __LR35902_NR32_H__