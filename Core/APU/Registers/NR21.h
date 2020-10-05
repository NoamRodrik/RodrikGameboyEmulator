/**
 * @file		NR21.h
 * @author		Noam Rodrik
 * @description LR35902's main apu NR21 definition header.
 */
#ifndef __LR35902_NR21_H__
#define __LR35902_NR21_H__

#include <Core/CPU/Registers/MemoryRegister.h>

namespace Core
{
class NR21 : public MemoryRegister<API::data_t>
{
public:
	constexpr NR21() : MemoryRegister{NR21_ADDRESS} {}
	NR21(const API::data_t value) : MemoryRegister{NR21_ADDRESS} { *this = value; }

public:
	using MemoryRegister::operator=;
	using MemoryRegister::operator API::data_t;

public:
	static constexpr API::address_t NR21_ADDRESS{0xFF16};
	static constexpr API::address_t NR21_DEFAULT_VALUE{0x3F};
};
} // Core

#endif // __LR35902_NR21_H__