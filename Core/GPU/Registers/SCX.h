/**
 * @file		SCX.h
 * @author		Noam Rodrik
 * @description LR35902's main gpu scx definition header.
 */
#ifndef __LR35902_SCX_H__
#define __LR35902_SCX_H__

#include <Core/CPU/Registers/MemoryRegister.h>

namespace Core
{
class SCX : public MemoryRegister<API::data_t>
{
public:
	constexpr SCX() : MemoryRegister{SCX_ADDRESS} {}
	SCX(const API::data_t value) : MemoryRegister{SCX_ADDRESS} { *this = value; }

public:
	using MemoryRegister::operator=;
	using MemoryRegister::operator API::data_t;

public:
	static constexpr API::address_t SCX_ADDRESS{0xFF43};
	static constexpr API::address_t SCX_DEFAULT_VALUE{0x00};
};
} // Core

#endif // __LR35902_SCX_H__