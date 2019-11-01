/**
 * @file		MemoryRegister.h
 * @author		Noam Rodrik
 * @description LR35902 main memory registers definition header.
 */
#ifndef __LR35902_MEMORY_REGISTER_H__
#define __LR35902_MEMORY_REGISTER_H__

#include <Core/Processor/Processor.h>

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
	MemoryRegister(const address_t address) : MEMORY_ADDRESS{address} {}

	MemoryRegister& operator=(const T other)
	{
		Processor::GetInstance().GetMemory().Write(other, MEMORY_ADDRESS);
		return *this;
	}

	operator T() const
	{
		T temporary_object;
		SANITY(Processor::GetInstance().GetMemory().Read(MEMORY_ADDRESS, temporary_object), "Failed reading from memory!");
		return temporary_object;
	}

private:
	const address_t MEMORY_ADDRESS;
};
} // Core

#endif // __LR35902_MEMORY_REGISTER_H__
