/**
 * @file		Cartridge.h
 * @author		Noam Rodrik
 * @description LR35902 main cartridge RAM device header.
 */
#ifndef __LR35902_MEMORY_DEVICE_CARTRIDGE_RAM_H__
#define __LR35902_MEMORY_DEVICE_CARTRIDGE_RAM_H__

#include <API/Memory/Device/IMemoryDeviceAccess.h>
#include <API/Memory/Device/IMemoryDevice.h>
#include <API/Memory/Memory.h>
#include <API/Definitions.h>
#include <algorithm>

namespace Core
{
class CartridgeRAM : public API::IMemoryDevice
{
public:
	CartridgeRAM(API::IMemoryDeviceAccess& _memory_accessor) : API::IMemoryDevice{START_ADDRESS, END_ADDRESS, _memory_accessor} {}

	virtual bool Read(const API::address_t absolute_address, API::data_t& result) const override
	{
		// While 0xFF50 isn't 0x1, we still return the system_boot code.
		if (!this->_covered_system_boot && (absolute_address - START_ADDRESS) <= 0xFF)
		{
			result = API::SYSTEM_BOOT_CODE[absolute_address - START_ADDRESS];
		}
		else
		{
			result = this->_memory[absolute_address - START_ADDRESS];
		}

		return true;
	}

	virtual bool Write(const API::address_t absolute_address, const API::data_t data) override
	{
		this->_memory[absolute_address - START_ADDRESS] = data;
		return true;
	}

	/**
	 * Will be called when the system boot code will be swapped with the cartridge code.
	 */
	inline void CoverSystemBoot() { this->_covered_system_boot = true; }

public:
	static constexpr API::address_t START_ADDRESS = 0x0000;
	static constexpr API::address_t END_ADDRESS = 0x7FFF;
	static constexpr size_t   SIZE = END_ADDRESS - START_ADDRESS + 1;

public:
	// Public for all the MBCS
	virtual uint8_t* GetMemoryPointer() override { return this->_memory.GetMemoryPointer(); }

private:
	API::Memory<SIZE> _memory;
	bool              _covered_system_boot{SKIP_BOOT ? true : false};

private:
	friend class DeviceManager;
};
} // Core

#endif // __LR35902_MEMORY_DEVICE_CARTRIDGE_RAM_H__
