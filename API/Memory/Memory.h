/**
 * @file		Memory.h
 * @author		Noam Rodrik
 * @description Defines basic actions for memory objects.
 */
#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <stdint.h>
#include <array>

namespace API
{
template <size_t SIZE>
class Memory
{
public:
	constexpr Memory() : m_ram{}
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
} // API

#endif // __MEMORY_H__