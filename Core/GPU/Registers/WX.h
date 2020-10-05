/**
 * @file		WX.h
 * @author		Noam Rodrik
 * @description LR35902's main gpu wx definition header.
 */
#ifndef __LR35902_WX_H__
#define __LR35902_WX_H__

#include <Core/CPU/Registers/MemoryRegister.h>

namespace Core
{
class WX : public MemoryRegister<API::data_t>
{
public:
	constexpr WX() : MemoryRegister{WX_ADDRESS} {}
	WX(const API::data_t value) : MemoryRegister{WX_ADDRESS} { *this = value; }

public:
	using MemoryRegister::operator=;
	using MemoryRegister::operator API::data_t;

public:
	static constexpr API::address_t WX_ADDRESS{0xFF4B};
	static constexpr API::address_t WX_DEFAULT_VALUE{0x00};
};
} // Core

#endif // __LR35902_WX_H__