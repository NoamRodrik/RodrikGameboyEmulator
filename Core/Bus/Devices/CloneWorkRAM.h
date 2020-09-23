/**
 * @file		CloneWorkRAM.h
 * @author		Noam Rodrik
 * @description LR35902 main clone work RAM device header.
 */
#ifndef __LR35902_MEMORY_DEVICE_CLONE_WORK_RAM_H__
#define __LR35902_MEMORY_DEVICE_CLONE_WORK_RAM_H__

#include <Core/Bus/RAMDevice.h>
#include <API/Definitions.h>

namespace Core
{
class WorkRAM;

class CloneWorkRAM : public RAMDevice<0xE000, 0xFDFF>
{
public:
	using RAMDevice::RAMDevice;

public:
	virtual bool Write(const API::address_t absolute_address, const API::data_t data) override
	{
		this->_memory[this->RelativeAddress(absolute_address)] = data;

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
