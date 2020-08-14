/**
 * @file		MemoryRegister.h
 * @author		Noam Rodrik
 * @description LR35902 main memory registers definition header.
 */
#ifndef __LR35902_MEMORY_REGISTER_H__
#define __LR35902_MEMORY_REGISTER_H__

#include <Core/CPU/Instructions/Shortcuts.h>
#include <Core/CPU/Processor.h>

namespace Core
{
/**
 * A templated class that serves as an abstraction of registers to their destination.
 * A register can now be on the RAM and act as something independent on itself.
 */
template <typename T>
class MemoryRegister
{
public:
	MemoryRegister(const API::address_t address) : MEMORY_ADDRESS{address} {}

	MemoryRegister& operator=(const T other)
	{
		Processor::GetInstance().GetMemory().Write(MEMORY_ADDRESS, other);
		return *this;
	}

	operator T() const
	{
		return READ_DATA_AT(MEMORY_ADDRESS);
	}

private:
	const API::address_t MEMORY_ADDRESS;
};
} // Core

#endif // __LR35902_MEMORY_REGISTER_H__
