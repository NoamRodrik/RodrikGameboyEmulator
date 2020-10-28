/**
 * @file		WorkRAMDevice.h
 * @author		Noam Rodrik
 * @description LR35902 main work RAM device header.
 */
#ifndef __LR35902_MEMORY_DEVICE_WORK_RAM_H__
#define __LR35902_MEMORY_DEVICE_WORK_RAM_H__

#include <Core/Bus/Devices/CloneWorkRAMDevice.h>
#include <Core/Bus/Devices/OAMRAMDevice.h>
#include <Core/Bus/MemoryDevice.h>
#include <API/Definitions.h>

namespace Core
{
class [[nodiscard]] WorkRAMDevice : public MemoryDevice<0xC000, 0xDFFF>
{
public:
	WorkRAMDevice(API::IMemoryDeviceAccess& memory_accessor) : MemoryDevice{memory_accessor} {}
	virtual ~WorkRAMDevice() override = default;

public:
	[[nodiscard]] virtual bool Write(const API::address_t absolute_address, const API::data_t data) override
	{
		RET_FALSE_IF_FAIL(MemoryDevice::Write(absolute_address, data), "Failed modifying data");

		// We don't want to randomly write into the other rams.
		if (CloneWorkRAMDevice::START_ADDRESS + this->RelativeAddress(absolute_address) < OAMRAMDevice::START_ADDRESS)
		{
			API::data_t clone_data{0};
			
			RET_FALSE_IF_FAIL(this->_memory_accessor.Read(CloneWorkRAMDevice::START_ADDRESS + this->RelativeAddress(absolute_address), clone_data),
							  "Failed reading from CloneWorkRAMDevice");

			if (clone_data != data)
			{
				// Clones to the CloneWorkRAMDevice
				return this->_memory_accessor.Write(CloneWorkRAMDevice::START_ADDRESS + this->RelativeAddress(absolute_address), data);
			}
		}

		return true;
	}

private:
	friend class DeviceManager;
};
} // Core

#endif // __LR35902_MEMORY_DEVICE_WORK_RAM_H__
