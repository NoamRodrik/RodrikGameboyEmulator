/**
 * @file		NR22.h
 * @author		Noam Rodrik
 * @description LR35902's main apu NR22 definition header.
 */
#ifndef __LR35902_NR22_H__
#define __LR35902_NR22_H__

#include <Core/CPU/Registers/MemoryRegister.h>

namespace Core
{
class [[nodiscard]] NR22 : public MemoryRegister<API::data_t>
{
public:
	constexpr NR22() : MemoryRegister{NR22_ADDRESS} {}
	NR22(const API::data_t value) : MemoryRegister{NR22_ADDRESS} { *this = value; }

public:
	using MemoryRegister::operator=;
	using MemoryRegister::operator API::data_t;

public:
	static constexpr API::address_t NR22_ADDRESS{0xFF17};
	static constexpr API::address_t NR22_DEFAULT_VALUE{0x00};
};
} // Core

#endif // __LR35902_NR22_H__