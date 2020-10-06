/**
 * @file		NR34.h
 * @author		Noam Rodrik
 * @description LR35902's main apu NR34 definition header.
 */
#ifndef __LR35902_NR34_H__
#define __LR35902_NR34_H__

#include <Core/CPU/Registers/MemoryRegister.h>

namespace Core
{
class NR34 : public MemoryRegister<API::data_t>
{
public:
	constexpr NR34() : MemoryRegister{NR34_ADDRESS} {}
	NR34(const API::data_t value) : MemoryRegister{NR34_ADDRESS} { *this = value; }

public:
	using MemoryRegister::operator=;
	using MemoryRegister::operator API::data_t;

public:
	static constexpr API::address_t NR34_ADDRESS{0xFF1E};
	static constexpr API::address_t NR34_DEFAULT_VALUE{0xBF};
};
} // Core

#endif // __LR35902_NR34_H__