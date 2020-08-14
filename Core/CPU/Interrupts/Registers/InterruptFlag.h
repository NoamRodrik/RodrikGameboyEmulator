/**
 * @file		InterruptFlag.h
 * @author		Noam Rodrik
 * @description LR35902 main interrupt flag definition header.
 */
#ifndef __LR35902_INTERRUPT_FLAG_H__
#define __LR35902_INTERRUPT_FLAG_H__

#include <Core/Registers/Memory/MemoryRegister.h>

namespace Core
{
class InterruptFlag : public MemoryRegister<data_t>
{
public:
	InterruptFlag() : MemoryRegister{INTERRUPT_FLAG_ADDRESS} {}
	InterruptFlag(const data_t value) : MemoryRegister{INTERRUPT_FLAG_ADDRESS} {*this = value;}

public:
	using MemoryRegister::operator=;
	using MemoryRegister::operator data_t;

public:
	static constexpr address_t INTERRUPT_FLAG_ADDRESS{0xFF0F};
	static constexpr data_t    INTERRUPT_FLAG_DEFAULT_VALUE{0x00};
};
} // Core

#endif // __LR35902_INTERRUPT_FLAG_H__