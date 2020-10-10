/**
 * @file		LY.h
 * @author		Noam Rodrik
 * @description LR35902's main gpu lyc definition header.
 */
#ifndef __LR35902_LYC_H__
#define __LR35902_LYC_H__

#include <Core/CPU/Registers/MemoryRegister.h>

namespace Core
{
class [[nodiscard]] LYC : public MemoryRegister<API::data_t>
{
public:
	constexpr LYC() : MemoryRegister{LYC_ADDRESS} {}
	LYC(const API::data_t value) : MemoryRegister{LYC_ADDRESS} { *this = value; }

public:
	using MemoryRegister::operator=;
	using MemoryRegister::operator API::data_t;

public:
	static constexpr API::address_t LYC_ADDRESS{0xFF45};
	static constexpr API::address_t LYC_DEFAULT_VALUE{0x00};
};
} // Core

#endif // __LR35902_LYC_H__