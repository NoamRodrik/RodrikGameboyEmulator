/**
 * @file		NR43.h
 * @author		Noam Rodrik
 * @description LR35902's main apu NR43 definition header.
 */
#ifndef __LR35902_NR43_H__
#define __LR35902_NR43_H__

#include <Core/CPU/Registers/MemoryRegister.h>

namespace Core
{
class NR43 : public MemoryRegister<API::data_t>
{
public:
	constexpr NR43() : MemoryRegister{NR43_ADDRESS} {}
	NR43(const API::data_t value) : MemoryRegister{NR43_ADDRESS} { *this = value; }

public:
	using MemoryRegister::operator=;
	using MemoryRegister::operator API::data_t;

public:
	static constexpr API::address_t NR43_ADDRESS{0xFF22};
	static constexpr API::address_t NR43_DEFAULT_VALUE{0x00};
};
}

#endif // __LR35902_NR43_H__