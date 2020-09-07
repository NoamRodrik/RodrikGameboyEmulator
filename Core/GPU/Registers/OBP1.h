/**
 * @file		OBP1.h
 * @author		Noam Rodrik
 * @description LR35902's main gpu obp1 definition header.
 */
#ifndef __LR35902_OBP1_H__
#define __LR35902_OBP1_H__

#include <Core/CPU/Registers/MemoryRegister.h>

namespace Core
{
class OBP1 : public MemoryRegister<API::data_t>
{
public:
	constexpr OBP1() : MemoryRegister{OBP1_ADDRESS} {}
	OBP1(const API::data_t value) : MemoryRegister{OBP1_ADDRESS} { *this = value; }

public:
	using MemoryRegister::operator=;
	using MemoryRegister::operator API::data_t;

public:
	static constexpr API::address_t OBP1_ADDRESS{0xFF49};
	static constexpr API::address_t OBP1_DEFAULT_VALUE{0xFF};
};
}

#endif // __LR35902_OBP1_H__