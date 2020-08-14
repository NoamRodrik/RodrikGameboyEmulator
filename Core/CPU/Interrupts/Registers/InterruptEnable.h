/**
 * @file		InterruptEnable.h
 * @author		Noam Rodrik
 * @description LR35902 main interrupt enable definition header.
 */
#ifndef __LR35902_INTERRUPT_ENABLE_H__
#define __LR35902_INTERRUPT_ENABLE_H__

#include <Core/Registers/Memory/MemoryRegister.h>

namespace Core
{
class InterruptEnable : public MemoryRegister<API::data_t>
{
public:
	InterruptEnable() : MemoryRegister{INTERRUPT_ENABLE_ADDRESS} {}
	InterruptEnable(const API::data_t value) : MemoryRegister{INTERRUPT_ENABLE_ADDRESS} { *this = value; }

public:
	using MemoryRegister::operator=;
	using MemoryRegister::operator API::data_t;

public:
	static constexpr API::address_t INTERRUPT_ENABLE_ADDRESS{0xFFFF};
	static constexpr API::data_t	INTERRUPT_ENABLE_DEFAULT_VALUE{0x00};
};
} // Core

#endif // __LR35902_INTERRUPT_ENABLE_H__