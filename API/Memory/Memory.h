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
	constexpr Memory() = default;
	~Memory() = default;

public:
	constexpr data_t& operator[](const size_t index)
	{
		return this->_ram[index];
	}

	constexpr const data_t& operator[](const size_t index) const
	{
		return this->_ram[index];
	}

	constexpr data_t* GetMemoryPointer() { return this->_ram.data(); }

	static constexpr size_t MEMORY_SIZE = SIZE;

private:
	std::array<data_t, SIZE> _ram{};
};
} // API

#endif // __MEMORY_H__