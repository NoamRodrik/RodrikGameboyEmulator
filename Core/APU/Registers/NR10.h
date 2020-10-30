/**
 * @file		NR10.h
 * @author		Noam Rodrik
 * @description LR35902's main apu NR10 definition header.
 */
#ifndef __LR35902_NR10_H__
#define __LR35902_NR10_H__

#include <Core/CPU/Registers/DeviceRegister.h>

namespace Core
{
class [[nodiscard]] NR10 : public DeviceRegister<0xFF10>
{
public:
	using DeviceRegister::DeviceRegister;
	using DeviceRegister::operator=;
	using DeviceRegister::operator API::data_t;

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
	static constexpr API::data_t SWEEP_TIME_BIT_INDEX{0x04};
	static constexpr API::data_t SWEEP_DIRECTION_BIT_INDEX{0x03};
	static constexpr API::data_t SWEEP_SHIFT_AMOUNT_BIT_INDEX{0x00};
	static constexpr API::data_t NR10_DEFAULT_VALUE{0x80};
};
} // Core

#endif // __LR35902_NR10_H__