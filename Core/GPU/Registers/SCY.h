/**
 * @file		SCY.h
 * @author		Noam Rodrik
 * @description LR35902's main gpu scy definition header.
 */
#ifndef __LR35902_SCY_H__
#define __LR35902_SCY_H__

#include <Core/CPU/Registers/MemoryRegister.h>

namespace Core
{
class SCY : public MemoryRegister<API::data_t>
{
public:
	constexpr SCY() : MemoryRegister{SCY_ADDRESS} {}
	SCY(const API::data_t value) : MemoryRegister{SCY_ADDRESS} { *this = value; }

public:
	using MemoryRegister::operator=;
	using MemoryRegister::operator API::data_t;

public:
	static constexpr API::address_t SCY_ADDRESS{0xFF42};
	static constexpr API::address_t SCY_DEFAULT_VALUE{0x00};
};
} // Core

#endif // __LR35902_SCY_H__