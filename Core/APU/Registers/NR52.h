/**
 * @file		NR52.h
 * @author		Noam Rodrik
 * @description LR35902's main apu NR52 definition header.
 */
#ifndef __LR35902_NR52_H__
#define __LR35902_NR52_H__

#include <Core/CPU/Registers/MemoryRegister.h>

namespace Core
{
class NR52 : public MemoryRegister<API::data_t>
{
public:
	constexpr NR52() : MemoryRegister{NR52_ADDRESS} {}
	NR52(const API::data_t value) : MemoryRegister{NR52_ADDRESS} { *this = value; }

public:
	using MemoryRegister::operator=;
	using MemoryRegister::operator API::data_t;

public:
	static constexpr API::address_t NR52_ADDRESS{0xFF26};
	static constexpr API::address_t NR52_DEFAULT_VALUE{0xF1};
};
} // Core

#endif // __LR35902_NR52_H__