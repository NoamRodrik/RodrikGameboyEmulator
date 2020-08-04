/**
 * @file		MemoryBase.h
 * @author		Noam Rodrik
 * @description Defines basic actions for memory base objects.
 */
#ifndef __MEMORY_BASE_H__
#define __MEMORY_BASE_H__

#include <stdint.h>
#include <array>

namespace Core
{
template <size_t SIZE>
class MemoryBase
{
public:
	constexpr MemoryBase() : m_ram{}
	{
		this->ClearRAM();
	}

	constexpr data_t& operator[](const address_t address)
	{
		return this->m_ram[address];
	}

	constexpr const data_t& operator[](const address_t address) const
	{
		return this->m_ram[address];
	}

	constexpr data_t* GetMemoryPointer() { return this->m_ram.data(); }

private:
	constexpr void ClearRAM()
	{
		for (auto&& current_data : this->m_ram)
		{
			current_data = 0;
		}
	}

private:
	std::array<data_t, SIZE> m_ram;
};
} // Core

#endif // __MEMORY_BASE_H__