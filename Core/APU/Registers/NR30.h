/**
 * @file		NR30.h
 * @author		Noam Rodrik
 * @description LR35902's main apu NR30 definition header.
 */
#ifndef __LR35902_NR30_H__
#define __LR35902_NR30_H__

#include <Core/CPU/Registers/MemoryRegister.h>

namespace Core
{
class NR30 : public MemoryRegister<API::data_t>
{
public:
	constexpr NR30() : MemoryRegister{NR30_ADDRESS} {}
	NR30(const API::data_t value) : MemoryRegister{NR30_ADDRESS} { *this = value; }

public:
	using MemoryRegister::operator=;
	using MemoryRegister::operator API::data_t;

public:
	static constexpr API::address_t NR30_ADDRESS{0xFF1A};
	static constexpr API::address_t NR30_DEFAULT_VALUE{0x7F};
};
} // Core

#endif // __LR35902_NR30_H__