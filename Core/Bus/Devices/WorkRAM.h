/**
 * @file		WorkRAM.h
 * @author		Noam Rodrik
 * @description LR35902 main work RAM device header.
 */
#ifndef __LR35902_MEMORY_DEVICE_WORK_RAM_H__
#define __LR35902_MEMORY_DEVICE_WORK_RAM_H__

#include <API/Memory/Device/IMemoryDeviceAccess.h>
#include <API/Memory/Device/IMemoryDevice.h>
#include <Core/Bus/Devices/CloneWorkRAM.h>
#include <Core/Bus/Devices/OAMRAM.h>
#include <API/Memory/Memory.h>
#include <API/Definitions.h>

namespace Core
{
class WorkRAM : public API::IMemoryDevice
{
public:
	constexpr WorkRAM(API::IMemoryDeviceAccess& memory_accessor) : API::IMemoryDevice{START_ADDRESS, END_ADDRESS, memory_accessor} {}

	virtual bool Read(const API::address_t absolute_address, API::data_t& result) const override
	{
		result = this->_memory[absolute_address - START_ADDRESS];

		return true;
	}

	virtual bool Write(const API::address_t absolute_address, const API::data_t data) override
	{
		this->_memory[GetFixedAddress(absolute_address)] = data;

		// We don't want to randomly write into the other rams.
		if (CloneWorkRAM::START_ADDRESS + GetFixedAddress(absolute_address) < OAMRAM::START_ADDRESS)
		{
			API::data_t clone_data{0};
			
			RET_FALSE_IF_FAIL(this->_memory_accessor.Read(CloneWorkRAM::START_ADDRESS + GetFixedAddress(absolute_address), clone_data),
							  "Failed reading from CloneWorkRAM");

			if (clone_data != data)
			{
				// Clones to the CloneWorkRAM
				return this->_memory_accessor.Write(CloneWorkRAM::START_ADDRESS + GetFixedAddress(absolute_address), data);
			}
		}

		return true;
	}

public:
	static constexpr API::address_t START_ADDRESS = 0xC000;
	static constexpr API::address_t END_ADDRESS = 0xDFFF;
	static constexpr size_t   SIZE = END_ADDRESS - START_ADDRESS + 1;

protected:
	virtual uint8_t* GetMemoryPointer() override { return this->_memory.GetMemoryPointer(); }

private:
	static constexpr API::address_t GetFixedAddress(const API::address_t address) { return address - START_ADDRESS; }

private:
	API::Memory<SIZE> _memory;

private:
	friend class DeviceManager;
};
} // Core

#endif // __LR35902_MEMORY_DEVICE_WORK_RAM_H__
