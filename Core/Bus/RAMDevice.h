/**
 * @file		RAMDevice.h
 * @author		Noam Rodrik
 * @description LR35902 main ram device header.
 */
#ifndef __LR35902_RAM_DEVICE_H__
#define __LR35902_RAM_DEVICE_H__

#include <API/Memory/Device/IMemoryDeviceAccess.h>
#include <API/Memory/Device/IMemoryDevice.h>
#include <Core/Bus/IRAMDevice.h>
#include <API/Memory/Memory.h>
#include <API/Definitions.h>

namespace Core
{
template <API::address_t START, API::address_t END>
class RAMDevice : public IRAMDevice, public API::IMemoryDevice
{
public:
	RAMDevice(API::IMemoryDeviceAccess& memory_accessor) : API::IMemoryDevice{START, END, memory_accessor} {}

	virtual ~RAMDevice() override = default;

public:
	/**
	 * Default implementation.
	 */
	virtual bool Read(const API::address_t absolute_address, API::data_t& result) const override
	{
		if (!this->InterceptRead(absolute_address, result))
		{
			result = this->_memory[this->RelativeAddress(absolute_address)];
		}

		return true;
	}

	/**
	 * Default implementation.
	 */
	virtual bool Write(const API::address_t absolute_address, const API::data_t data) override
	{
		if (!this->InterceptWrite(absolute_address, data))
		{
#ifdef TETRIS_PATCH
			if (absolute_address != 0xFF80)
#endif
			this->_memory[this->RelativeAddress(absolute_address)] = data;
		}

		return true;
	}

protected:
	virtual bool InterceptWrite(const API::address_t absolute_address, const API::data_t data) override
	{
		return false;
	}

	virtual bool InterceptRead(const API::address_t absolute_address, API::data_t& result) const override
	{
		return false;
	}

public:
	static constexpr API::address_t START_ADDRESS = START;
	static constexpr API::address_t END_ADDRESS = END;
	static constexpr size_t   SIZE = END_ADDRESS - START_ADDRESS + 1;

protected:
	static constexpr API::address_t RelativeAddress(const API::address_t address) { return address - START; }
	virtual uint8_t* GetMemoryPointer() override { return this->_memory.GetMemoryPointer(); }

protected:
	API::Memory<SIZE> _memory;

private:
	friend class DeviceManager;
};
} // Core

#endif // __LR35902_RAM_DEVICE_H__
