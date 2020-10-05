/**
 * @file		NR33.h
 * @author		Noam Rodrik
 * @description LR35902's main apu NR33 definition header.
 */
#ifndef __LR35902_NR33_H__
#define __LR35902_NR33_H__

#include <Core/CPU/Registers/MemoryRegister.h>

namespace Core
{
class NR33 : public MemoryRegister<API::data_t>
{
public:
	constexpr NR33() : MemoryRegister{NR33_ADDRESS} {}
	NR33(const API::data_t value) : MemoryRegister{NR33_ADDRESS} { *this = value; }

public:
	using MemoryRegister::operator=;
	using MemoryRegister::operator API::data_t;

public:
	static constexpr API::address_t NR33_ADDRESS{0xFF1D};
	static constexpr API::address_t NR33_DEFAULT_VALUE{0x00};
};
}

#endif // __LR35902_NR33_H__