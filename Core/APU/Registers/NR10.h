/**
 * @file		NR10.h
 * @author		Noam Rodrik
 * @description LR35902's main apu NR10 definition header.
 */
#ifndef __LR35902_NR10_H__
#define __LR35902_NR10_H__

#include <Core/CPU/Registers/MemoryRegister.h>

namespace Core
{
class [[nodiscard]] NR10 : public MemoryRegister<API::data_t>
{
public:
	constexpr NR10() : MemoryRegister{NR10_ADDRESS} {}
	NR10(const API::data_t value) : MemoryRegister{NR10_ADDRESS} { *this = value; }

public:
	using MemoryRegister::operator=;
	using MemoryRegister::operator API::data_t;

public:
	static constexpr API::address_t NR10_ADDRESS{0xFF10};
	static constexpr API::address_t NR10_DEFAULT_VALUE{0x80};
};
} // Core

#endif // __LR35902_NR10_H__