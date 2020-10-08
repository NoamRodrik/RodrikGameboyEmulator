/**
 * @file		NR44.h
 * @author		Noam Rodrik
 * @description LR35902's main apu NR44 definition header.
 */
#ifndef __LR35902_NR44_H__
#define __LR35902_NR44_H__

#include <Core/CPU/Registers/MemoryRegister.h>

namespace Core
{
class [[nodiscard]] NR44 : public MemoryRegister<API::data_t>
{
public:
	constexpr NR44() : MemoryRegister{NR44_ADDRESS} {}
	NR44(const API::data_t value) : MemoryRegister{NR44_ADDRESS} { *this = value; }

public:
	using MemoryRegister::operator=;
	using MemoryRegister::operator API::data_t;

public:
	static constexpr API::address_t NR44_ADDRESS{0xFF23};
	static constexpr API::address_t NR44_DEFAULT_VALUE{0xBF};
};
} // Core

#endif // __LR35902_NR44_H__