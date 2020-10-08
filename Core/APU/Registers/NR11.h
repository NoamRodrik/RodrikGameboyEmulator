/**
 * @file		NR11.h
 * @author		Noam Rodrik
 * @description LR35902's main apu NR11 definition header.
 */
#ifndef __LR35902_NR11_H__
#define __LR35902_NR11_H__

#include <Core/CPU/Registers/MemoryRegister.h>

namespace Core
{
class NR11 : public MemoryRegister<API::data_t>
{
public:
	constexpr NR11() : MemoryRegister{NR11_ADDRESS} {}
	NR11(const API::data_t value) : MemoryRegister{NR11_ADDRESS} { *this = value; }

public:
	using MemoryRegister::operator=;
	using MemoryRegister::operator API::data_t;

public:
	static constexpr API::data_t    SEQUENCE_BIT{0x06};
	static constexpr API::address_t NR11_ADDRESS{0xFF11};
	static constexpr API::address_t NR11_DEFAULT_VALUE{0xBF};
};
} // Core

#endif // __LR35902_NR11_H__