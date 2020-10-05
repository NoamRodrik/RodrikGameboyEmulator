/**
 * @file		NR50.h
 * @author		Noam Rodrik
 * @description LR35902's main apu NR50 definition header.
 */
#ifndef __LR35902_NR50_H__
#define __LR35902_NR50_H__

#include <Core/CPU/Registers/MemoryRegister.h>

namespace Core
{
class NR50 : public MemoryRegister<API::data_t>
{
public:
	constexpr NR50() : MemoryRegister{NR50_ADDRESS} {}
	NR50(const API::data_t value) : MemoryRegister{NR50_ADDRESS} { *this = value; }

public:
	using MemoryRegister::operator=;
	using MemoryRegister::operator API::data_t;

public:
	static constexpr API::address_t NR50_ADDRESS{0xFF24};
	static constexpr API::address_t NR50_DEFAULT_VALUE{0x77};
};
} // Core

#endif // __LR35902_NR50_H__