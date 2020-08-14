/**
 * @file		DividerRegister.h
 * @author		Noam Rodrik
 * @description LR35902 main divider register definition header.
 */
#ifndef __LR35902_DIVIDER_REGISTER_H__
#define __LR35902_DIVIDER_REGISTER_H__

#include <Core/CPU/Registers/MemoryRegister.h>

namespace Core
{
class DividerRegister : public MemoryRegister<API::data_t>
{
public:
	DividerRegister() : MemoryRegister{DIVIDER_REGISTER_ADDRESS} {}
	DividerRegister(const API::data_t value) : MemoryRegister{DIVIDER_REGISTER_ADDRESS} { *this = value; }

public:
	using MemoryRegister::operator=;
	using MemoryRegister::operator API::data_t;

public:
	static constexpr API::address_t DIVIDER_REGISTER_ADDRESS{0xFF04};
};
} // Core

#endif // __LR35902_DIVIDER_REGISTER_H__