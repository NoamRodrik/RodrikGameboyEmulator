/**
 * @file		NR13.h
 * @author		Noam Rodrik
 * @description LR35902's main apu NR13 definition header.
 */
#ifndef __LR35902_NR13_H__
#define __LR35902_NR13_H__

#include <Core/CPU/Registers/MemoryRegister.h>

namespace Core
{
class [[nodiscard]] NR13 : public MemoryRegister<API::data_t>
{
public:
	constexpr NR13() : MemoryRegister{NR13_ADDRESS} {}
	NR13(const API::data_t value) : MemoryRegister{NR13_ADDRESS} { *this = value; }

public:
	using MemoryRegister::operator=;
	using MemoryRegister::operator API::data_t;

public:
	static constexpr API::address_t NR13_ADDRESS{0xFF13};
	static constexpr API::address_t NR13_DEFAULT_VALUE{0x00};
};
} // Core

#endif // __LR35902_NR13_H__