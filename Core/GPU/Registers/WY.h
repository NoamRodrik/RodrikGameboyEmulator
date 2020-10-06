/**
 * @file		WY.h
 * @author		Noam Rodrik
 * @description LR35902's main gpu wy definition header.
 */
#ifndef __LR35902_WY_H__
#define __LR35902_WY_H__

#include <Core/CPU/Registers/MemoryRegister.h>

namespace Core
{
class WY : public MemoryRegister<API::data_t>
{
public:
	constexpr WY() : MemoryRegister{WY_ADDRESS} {}
	WY(const API::data_t value) : MemoryRegister{WY_ADDRESS} { *this = value; }

public:
	using MemoryRegister::operator=;
	using MemoryRegister::operator API::data_t;

public:
	static constexpr API::address_t WY_ADDRESS{0xFF4A};
	static constexpr API::address_t WY_DEFAULT_VALUE{0x00};
};
} // Core

#endif // __LR35902_WY_H__