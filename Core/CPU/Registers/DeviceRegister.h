/**
 * @file		DeviceRegister.h
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
template <API::address_t ADDRESS>
class [[nodiscard]] DeviceRegister
{
public:
	constexpr DeviceRegister() = default;
	DeviceRegister(const API::data_t value) { *this = value; }

public:
	DeviceRegister& operator=(const API::data_t other)
	{
		static gsl::not_null<Processor*> processor{&Processor::GetInstance()};
		SANITY(processor->GetMemory().Write(MEMORY_ADDRESS, other), "Failed writing to memory");
		return *this;
	}

	operator API::data_t() const
	{
		return READ_DATA_AT(MEMORY_ADDRESS);
	}

public:
	static constexpr API::address_t MEMORY_ADDRESS{ADDRESS};
};
} // Core

#endif // __LR35902_MEMORY_REGISTER_H__
