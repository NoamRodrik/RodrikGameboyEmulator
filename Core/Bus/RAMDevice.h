/**
 * @file		RAMDevice.h
 * @author		Noam Rodrik
 * @description LR35902 main ram device header.
 */
#ifndef __LR35902_RAM_DEVICE_H__
#define __LR35902_RAM_DEVICE_H__

#include <API/Memory/Device/IMemoryDeviceAccess.h>
#include <API/Memory/Device/IMemoryDevice.h>
#include <API/Memory/Memory.h>
#include <API/Definitions.h>

namespace Core
{
template <API::address_t START, API::address_t END>
class RAMDevice : public API::IMemoryDevice
{
public:
	constexpr RAMDevice(API::IMemoryDeviceAccess& memory_accessor) : API::IMemoryDevice{START, END, memory_accessor} {}

public:
	/**
	 * Default implementation.
	 */
	virtual bool Read(const API::address_t absolute_address, API::data_t& result) const override
	{
		result = this->_memory[this->RelativeAddress(absolute_address)];
		return true;
	}

	/**
	 * Default implementation.
	 */
	virtual bool Write(const API::address_t absolute_address, const API::data_t data) override
	{
		this->_memory[this->RelativeAddress(absolute_address)] = data;
		return true;
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
