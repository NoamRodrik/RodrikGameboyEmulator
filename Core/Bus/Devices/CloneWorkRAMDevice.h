/**
 * @file		CloneWorkRAMDevice.h
 * @author		Noam Rodrik
 * @description LR35902 main clone work RAM device header.
 */
#ifndef __LR35902_MEMORY_DEVICE_CLONE_WORK_RAM_H__
#define __LR35902_MEMORY_DEVICE_CLONE_WORK_RAM_H__

#include <Core/Bus/MemoryDevice.h>
#include <API/Definitions.h>

namespace Core
{
class WorkRAMDevice;

class [[nodiscard]] CloneWorkRAMDevice : public MemoryDevice<0xE000, 0xFDFF>
{
public:
	CloneWorkRAMDevice(API::IMemoryDeviceAccess& memory_accessor) : MemoryDevice{memory_accessor} {}
	virtual ~CloneWorkRAMDevice() override = default;

public:
	[[nodiscard]] virtual bool Write(const API::address_t absolute_address, const API::data_t data) override
	{
		RET_FALSE_IF_FAIL(MemoryDevice::Write(absolute_address, data), "Failed writing CloneWorkRAMDevice data");

		API::data_t work_ram_data{0};

		RET_FALSE_IF_FAIL(this->_memory_accessor.Read(0xC000 + this->RelativeAddress(absolute_address), work_ram_data),
						  "Failed reading from WorkRAM");

		if (work_ram_data != data)
		{
			return this->_memory_accessor.Write(0xC000 + this->RelativeAddress(absolute_address), work_ram_data);
		}

		return true;
	}

private:
	friend class DeviceManager;
};
} // Core

#endif // __LR35902_MEMORY_DEVICE_CLONE_WORK_RAM_H__
