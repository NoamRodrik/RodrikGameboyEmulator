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

	constexpr data_t& operator[](const size_t index)
	{
		return this->m_ram[index];
	}

	constexpr const data_t& operator[](const size_t index) const
	{
		return this->m_ram[index];
	}

	constexpr data_t* GetMemoryPointer() { return this->m_ram.data(); }

	static constexpr size_t MEMORY_SIZE = SIZE;

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