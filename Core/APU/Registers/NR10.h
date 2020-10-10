/**
 * @file		NR10.h
 * @author		Noam Rodrik
 * @description LR35902's main apu NR10 definition header.
 */
#ifndef __LR35902_NR10_H__
#define __LR35902_NR10_H__

#include <Core/CPU/Registers/MemoryRegister.h>

namespace Core
{
class [[nodiscard]] NR10 : public MemoryRegister<API::data_t>
{
public:
	constexpr NR10() : MemoryRegister{NR10_ADDRESS} {}
	NR10(const API::data_t value) : MemoryRegister{NR10_ADDRESS} { *this = value; }

public:
	using MemoryRegister::operator=;
	using MemoryRegister::operator API::data_t;

public:
	[[nodiscard]] const API::data_t GetSweepTime() const
	{
		return ((this->operator API::data_t()) >> SWEEP_TIME_BIT_INDEX) & 0b111;
	}

	[[nodiscard]] const bool IsSweepDecreasing() const
	{
		return ((this->operator API::data_t()) >> SWEEP_DIRECTION_BIT_INDEX) & 0b01;
	}

	[[nodiscard]] const bool IsSweepIncreasing() const
	{
		return !this->IsSweepDecreasing();
	}

	[[nodiscard]] const API::data_t GetSweepShiftAmount() const
	{
		return ((this->operator API::data_t()) >> SWEEP_SHIFT_AMOUNT_BIT_INDEX) & 0b111;
	}

public:
	static constexpr API::data_t    SWEEP_TIME_BIT_INDEX{0x04};
	static constexpr API::data_t    SWEEP_DIRECTION_BIT_INDEX{0x03};
	static constexpr API::data_t	SWEEP_SHIFT_AMOUNT_BIT_INDEX{0x00};
	static constexpr API::address_t NR10_ADDRESS{0xFF10};
	static constexpr API::address_t NR10_DEFAULT_VALUE{0x80};
};
} // Core

#endif // __LR35902_NR10_H__