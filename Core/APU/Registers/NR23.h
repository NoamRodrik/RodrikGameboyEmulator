/**
 * @file		NR23.h
 * @author		Noam Rodrik
 * @description LR35902's main apu NR23 definition header.
 */
#ifndef __LR35902_NR23_H__
#define __LR35902_NR23_H__

#include <Core/CPU/Registers/MemoryRegister.h>

namespace Core
{
class NR23 : public MemoryRegister<API::data_t>
{
public:
	constexpr NR23() : MemoryRegister{NR23_ADDRESS} {}
	NR23(const API::data_t value) : MemoryRegister{NR23_ADDRESS} { *this = value; }

public:
	using MemoryRegister::operator=;
	using MemoryRegister::operator API::data_t;

public:
	static constexpr API::address_t NR23_ADDRESS{0xFF18};
	static constexpr API::address_t NR23_DEFAULT_VALUE{0x00};
};
}

#endif // __LR35902_NR23_H__