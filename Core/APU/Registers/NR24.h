/**
 * @file		NR24.h
 * @author		Noam Rodrik
 * @description LR35902's main apu NR24 definition header.
 */
#ifndef __LR35902_NR24_H__
#define __LR35902_NR24_H__

#include <Core/CPU/Registers/MemoryRegister.h>

namespace Core
{
class NR24 : public MemoryRegister<API::data_t>
{
public:
	constexpr NR24() : MemoryRegister{NR24_ADDRESS} {}
	NR24(const API::data_t value) : MemoryRegister{NR24_ADDRESS} { *this = value; }

public:
	using MemoryRegister::operator=;
	using MemoryRegister::operator API::data_t;

public:
	static constexpr API::address_t NR24_ADDRESS{0xFF19};
	static constexpr API::address_t NR24_DEFAULT_VALUE{0xBF};
};
}

#endif // __LR35902_NR24_H__