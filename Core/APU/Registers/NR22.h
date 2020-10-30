/**
 * @file		NR22.h
 * @author		Noam Rodrik
 * @description LR35902's main apu NR22 definition header.
 */
#ifndef __LR35902_NR22_H__
#define __LR35902_NR22_H__

#include <Core/CPU/Registers/DeviceRegister.h>

namespace Core
{
class [[nodiscard]] NR22 : public DeviceRegister<0xFF17>
{
public:
	using DeviceRegister::DeviceRegister;
	using DeviceRegister::operator=;
	using DeviceRegister::operator API::data_t;

public:
	[[nodiscard]] const API::data_t GetEnvelopeSweep() const
	{
		return ((this->operator API::data_t()) >> NR22_ENVELOPE_SWEEP_BIT_INDEX) & 0b111;
	}

	[[nodiscard]] const bool GetEnvelopeDirection() const
	{
		return ((this->operator API::data_t()) >> NR22_ENVELOPE_DIRECTION_BIT_INDEX) & 0b01;
	}

	[[nodiscard]] const API::data_t GetEnvelopeVolume() const
	{
		return ((this->operator API::data_t()) >> NR22_ENVELOPE_VOLUME_BIT_INDEX) & 0b1111;
	}

public:
	static constexpr API::data_t NR22_ENVELOPE_VOLUME_BIT_INDEX{0x04};
	static constexpr API::data_t NR22_ENVELOPE_DIRECTION_BIT_INDEX{0x03};
	static constexpr API::data_t NR22_ENVELOPE_SWEEP_BIT_INDEX{0x00};
	static constexpr API::data_t NR22_DEFAULT_VALUE{0x00};
};
} // Core

#endif // __LR35902_NR22_H__