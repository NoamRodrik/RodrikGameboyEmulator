/**
 * @file		NR31.h
 * @author		Noam Rodrik
 * @description LR35902's main apu NR31 definition header.
 */
#ifndef __LR35902_NR31_H__
#define __LR35902_NR31_H__

#include <Core/CPU/Registers/MemoryRegister.h>

namespace Core
{
class NR31 : public MemoryRegister<API::data_t>
{
public:
	constexpr NR31() : MemoryRegister{NR31_ADDRESS} {}
	NR31(const API::data_t value) : MemoryRegister{NR31_ADDRESS} { *this = value; }

public:
	using MemoryRegister::operator=;
	using MemoryRegister::operator API::data_t;

public:
	static constexpr API::address_t NR31_ADDRESS{0xFF1B};
	static constexpr API::address_t NR31_DEFAULT_VALUE{0xFF};
};
}

#endif // __LR35902_NR31_H__