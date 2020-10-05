/**
 * @file		NR42.h
 * @author		Noam Rodrik
 * @description LR35902's main apu NR42 definition header.
 */
#ifndef __LR35902_NR42_H__
#define __LR35902_NR42_H__

#include <Core/CPU/Registers/MemoryRegister.h>

namespace Core
{
class NR42 : public MemoryRegister<API::data_t>
{
public:
	constexpr NR42() : MemoryRegister{NR42_ADDRESS} {}
	NR42(const API::data_t value) : MemoryRegister{NR42_ADDRESS} { *this = value; }

public:
	using MemoryRegister::operator=;
	using MemoryRegister::operator API::data_t;

public:
	static constexpr API::address_t NR42_ADDRESS{0xFF21};
	static constexpr API::address_t NR42_DEFAULT_VALUE{0x00};
};
}

#endif // __LR35902_NR42_H__