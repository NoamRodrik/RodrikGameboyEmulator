/**
 * @file		NR41.h
 * @author		Noam Rodrik
 * @description LR35902's main apu NR41 definition header.
 */
#ifndef __LR35902_NR41_H__
#define __LR35902_NR41_H__

#include <Core/CPU/Registers/MemoryRegister.h>

namespace Core
{
class [[nodiscard]] NR41 : public MemoryRegister<API::data_t>
{
public:
	constexpr NR41() : MemoryRegister{NR41_ADDRESS} {}
	NR41(const API::data_t value) : MemoryRegister{NR41_ADDRESS} { *this = value; }

public:
	using MemoryRegister::operator=;
	using MemoryRegister::operator API::data_t;

public:
	static constexpr API::address_t NR41_ADDRESS{0xFF20};
	static constexpr API::address_t NR41_DEFAULT_VALUE{0xFF};
};
} // Core

#endif // __LR35902_NR41_H__