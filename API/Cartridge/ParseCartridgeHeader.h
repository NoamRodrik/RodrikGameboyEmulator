/**
 * @file		Header.h
 * @author		Noam Rodrik
 * @description API of a generic memory bank controller.
 */
#ifndef __API_CARTRIDGE_HEADER_H__
#define __API_CARTRIDGE_HEADER_H__

#include <cstdint>

namespace API
{
class CartridgeHeader
{
public:
	CartridgeHeader() = default;
	~CartridgeHeader() = default;

public:
	void Parse();

	inline const CartridgeType Type() const
	{
		return this->m_type;
	}

public:
	enum class CartridgeType : std::uint8_t
	{
		ROM = 0x00,
		MBC1 = 0x01,
		MBC1_RAM = 0x02,
		MBC1_RAM_BATTERY = 0x03,
		MBC2 = 0x05,
		MBC2_BATTERY = 0x06,
		ROM_RAM = 0x08,
		ROM_RAM_BATTERY = 0x09,
		MMM01 = 0x0B,
		MMM01_RAM = 0x0C,
		MMM01_RAM_BATTERY = 0x0D,
		MBC3_TIMER_BATTERY = 0x0F,
		MBC3_TIMER_RAM_BATTERY = 0x10,
		MBC3 = 0x11,
		MBC3_RAM = 0x12,
		MBC3_RAM_BATTERY = 0x13,
		MBC5 = 0x19,
		MBC5_RAM = 0x1A,
		MBC5_RAM_BATTERY = 0x1B,
		MBC5_RUMBLE = 0x1C,
		MBC5_RUMBLE_RAM = 0x1D,
		MBC5_RUMBLE_RAM_BATTERY = 0x1E,
		MBC6 = 0x20,
		MBC7_SENSOR_RUMBLE_RAM_BATTERY = 0x22,
		POCKER_CAMERA = 0xFC,
		BANDAI_TAMA5 = 0xFD,
		HUC3 = 0xFE,
		HUC1_RAM_BATTERY = 0xFF
	};

private:
	CartridgeType m_type{CartridgeType::ROM};
};
}

#endif // __API_CARTRIDGE_HEADER_H__