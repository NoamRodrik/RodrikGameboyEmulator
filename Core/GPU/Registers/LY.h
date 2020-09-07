/**
 * @file		LY.h
 * @author		Noam Rodrik
 * @description LR35902's main gpu ly definition header.
 */
#ifndef __LR35902_LY_H__
#define __LR35902_LY_H__

#include <Core/CPU/Registers/MemoryRegister.h>

namespace Core
{
/* Read only! */
class LY : public MemoryRegister<API::data_t>
{
public:
	constexpr LY() : MemoryRegister{LY_ADDRESS} {}
	LY(const API::data_t value) : MemoryRegister{LY_ADDRESS} { *this = value; }

public:
	using MemoryRegister::operator=;
	using MemoryRegister::operator API::data_t;

public:
	static constexpr API::address_t LY_ADDRESS{0xFF44};
	static constexpr API::address_t LY_DEFAULT_VALUE{0x90};
};
}

#endif // __LR35902_LY_H__