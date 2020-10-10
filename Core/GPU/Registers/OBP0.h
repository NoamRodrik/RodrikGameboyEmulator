/**
 * @file		OBP0.h
 * @author		Noam Rodrik
 * @description LR35902's main gpu obp0 definition header.
 */
#ifndef __LR35902_OBP0_H__
#define __LR35902_OBP0_H__

#include <Core/CPU/Registers/MemoryRegister.h>

namespace Core
{
class [[nodiscard]] OBP0 : public MemoryRegister<API::data_t>
{
public:
	constexpr OBP0() : MemoryRegister{OBP0_ADDRESS} {}
	OBP0(const API::data_t value) : MemoryRegister{OBP0_ADDRESS} { *this = value; }

public:
	using MemoryRegister::operator=;
	using MemoryRegister::operator API::data_t;

public:
	static constexpr API::address_t OBP0_ADDRESS{0xFF48};
	static constexpr API::address_t OBP0_DEFAULT_VALUE{0xFF};
};
} // Core

#endif // __LR35902_OBP0_H__