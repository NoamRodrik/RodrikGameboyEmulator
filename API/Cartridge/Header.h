/**
 * @file		Header.h
 * @author		Noam Rodrik
 * @description API of a generic memory bank controller.
 */
#ifndef __API_CARTRIDGE_HEADER_H__
#define __API_CARTRIDGE_HEADER_H__

#include <API/Memory/Device/IMemoryDeviceAccess.h>
#include <Contrib/GSL/not_null.h>
#include <cstdint>

namespace API
{
class CartridgeHeader : public API::IMemoryDeviceAccess
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
		_1000_KB = 1024,
		_2000_KB = 2048,
		_4000_KB = 4096,
		_8000_KB = 8192,
		_1100_KB = 1152,
		_1200_KB = 1280,
		_1500_KB = 1536
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
	explicit CartridgeHeader(gsl::not_null<IMemoryDeviceAccess*> device_access) : _device_access{device_access} {}
	explicit CartridgeHeader(gsl::not_null<API::data_t*> data) : _data{data} {}
	~CartridgeHeader() = default;

public:
	inline const API::address_t GlobalChecksum() const
	{
		return static_cast<API::address_t>(CartridgeHeader::Fetch<GLOBAL_CHECKSUM_OFFSET>()) << 8 |
			   static_cast<API::address_t>(CartridgeHeader::Fetch<GLOBAL_CHECKSUM_OFFSET + 1>());
	}

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

	const ROMSizeValue ROMSize() const;
	const RAMSizeValue RAMSize() const;

private:
	template <std::size_t OFFSET>
	inline const data_t Fetch() const
	{
		data_t data{0};
		SANITY(this->Read(OFFSET, data),
			   "Failed to read from device access");
		return data;
	}

	virtual bool Read(const API::address_t absolute_address, API::data_t& result) const override
	{
		if (this->_device_access != nullptr)
		{
			return this->_device_access->Read(absolute_address, result);
		}

		// Can't be null.
		RET_FALSE_IF_FAIL(this->_data != nullptr, "Both device access and data are null in cartridge header.");
		result = this->_data[absolute_address];
		return true;
	}

	virtual bool Write(const API::address_t absolute_address, const API::data_t data) override
	{
		STOP_RUNNING("Can't write to cartridge header");
		return false;
	}

	virtual bool WriteDirectly(const API::address_t absolute_address, const API::data_t data) override
	{
		STOP_RUNNING("Can't write to cartridge header");
		return false;
	}

private:
	static constexpr std::size_t CARTRIDGE_TYPE_OFFSET{0x0147};
	static constexpr std::size_t ROM_SIZE_OFFSET{0x0148};
	static constexpr std::size_t RAM_SIZE_OFFSET{0x0149};
	static constexpr std::size_t GLOBAL_CHECKSUM_OFFSET{0x014E};

private:
	IMemoryDeviceAccess* _device_access{nullptr};
	API::data_t*         _data{nullptr};
};
} // API

#endif // __API_CARTRIDGE_HEADER_H__