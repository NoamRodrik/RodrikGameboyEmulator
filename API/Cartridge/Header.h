/**
 * @file		Header.h
 * @author		Noam Rodrik
 * @description API of a generic memory bank controller.
 */
#ifndef __API_CARTRIDGE_HEADER_H__
#define __API_CARTRIDGE_HEADER_H__

#include <API/Memory/Device/IMemoryDeviceAccess.h>
#include <cstdint>

namespace API
{
class CartridgeHeader
{
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

	enum class ROMSizeType : std::uint8_t
	{
		_32_KB = 0x00,
		_64_KB = 0x01,
		_128_KB = 0x02,
		_256_KB = 0x03,
		_512_KB = 0x04,
		_1000_KB = 0x05,
		_2000_KB = 0x06,
		_4000_KB = 0x07,
		_8000_KB = 0x08,
		_1100_KB = 0x52,
		_1200_KB = 0x53,
		_1500_KB = 0x54
	};

	enum class ROMSizeValue : std::uint16_t
	{
		_32_KB = 32,
		_64_KB = 64,
		_128_KB = 128,
		_256_KB = 256,
		_512_KB = 512,
		_1000_KB = 1000,
		_2000_KB = 2000,
		_4000_KB = 4000,
		_8000_KB = 8000,
		_1100_KB = 1100,
		_1200_KB = 1200,
		_1500_KB = 1500
	};

	enum class RAMSizeType : std::uint8_t
	{
		NONE = 0x00,
		_2_KB = 0x01,
		_8_KB = 0x02,
		_32_KB = 0x03,
		_128_KB = 0x04,
		_64_KB = 0x05
	};

	enum class RAMSizeValue : std::uint16_t
	{
		NONE = 0,
		_2_KB = 2,
		_8_KB = 8,
		_32_KB = 32,
		_128_KB = 128,
		_64_KB = 64
	};

public:
	CartridgeHeader(IMemoryDeviceAccess& device_access) : m_device_access(device_access) {}
	~CartridgeHeader() = default;

public:
	inline const CartridgeType MBC() const
	{
		return static_cast<CartridgeType>(CartridgeHeader::Fetch<CARTRIDGE_TYPE_OFFSET>());
	}

	inline const ROMSizeType ROM() const
	{
		return static_cast<ROMSizeType>(CartridgeHeader::Fetch<ROM_SIZE_OFFSET>());
	}

	inline const RAMSizeType RAM() const
	{
		return static_cast<RAMSizeType>(CartridgeHeader::Fetch<RAM_SIZE_OFFSET>());
	}

	inline const CartridgeHeader::ROMSizeValue CartridgeHeader::ROMSize() const;
	inline const CartridgeHeader::RAMSizeValue CartridgeHeader::RAMSize() const;

private:
	template <std::size_t OFFSET>
	inline const data_t Fetch() const
	{
		data_t data{0};
		SANITY(this->m_device_access.Read(OFFSET, data),
			   "Failed to read from device access");
		return data;
	}

private:
	static constexpr std::size_t CARTRIDGE_TYPE_OFFSET{0x0147};
	static constexpr std::size_t ROM_SIZE_OFFSET{0x0148};
	static constexpr std::size_t RAM_SIZE_OFFSET{0x0149};

private:
	IMemoryDeviceAccess& m_device_access;
};
}

#endif // __API_CARTRIDGE_HEADER_H__