/**
 * @file		NR12.h
 * @author		Noam Rodrik
 * @description LR35902's main apu NR12 definition header.
 */
#ifndef __LR35902_NR12_H__
#define __LR35902_NR12_H__

#include <Core/CPU/Registers/MemoryRegister.h>

namespace Core
{
class NR12 : public MemoryRegister<API::data_t>
{
public:
	constexpr NR12() : MemoryRegister{NR12_ADDRESS} {}
	NR12(const API::data_t value) : MemoryRegister{NR12_ADDRESS} { *this = value; }

public:
	using MemoryRegister::operator=;
	using MemoryRegister::operator API::data_t;

public:
	static constexpr API::address_t NR12_ADDRESS{0xFF12};
	static constexpr API::address_t NR12_DEFAULT_VALUE{0xF3};
};
} // Core

#endif // __LR35902_NR12_H__