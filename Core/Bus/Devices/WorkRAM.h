/**
 * @file		WorkRAM.h
 * @author		Noam Rodrik
 * @description LR35902 main work RAM device header.
 */
#ifndef __LR35902_MEMORY_DEVICE_WORK_RAM_H__
#define __LR35902_MEMORY_DEVICE_WORK_RAM_H__

#include <Core/Bus/Devices/CloneWorkRAM.h>
#include <Core/Bus/Devices/OAMRAM.h>
#include <Core/Bus/RAMDevice.h>
#include <API/Definitions.h>

namespace Core
{
class [[nodiscard]] WorkRAM : public RAMDevice<0xC000, 0xDFFF>
{
public:
	WorkRAM(API::IMemoryDeviceAccess& memory_accessor) : RAMDevice{memory_accessor} {}
	virtual ~WorkRAM() override = default;

public:
	[[nodiscard]] virtual bool Write(const API::address_t absolute_address, const API::data_t data) override
	{
		RET_FALSE_IF_FAIL(RAMDevice::Write(absolute_address, data), "Failed modifying data");

		// We don't want to randomly write into the other rams.
		if (CloneWorkRAM::START_ADDRESS + this->RelativeAddress(absolute_address) < OAMRAM::START_ADDRESS)
		{
			API::data_t clone_data{0};
			
			RET_FALSE_IF_FAIL(this->_memory_accessor.Read(CloneWorkRAM::START_ADDRESS + this->RelativeAddress(absolute_address), clone_data),
							  "Failed reading from CloneWorkRAM");

			if (clone_data != data)
			{
				// Clones to the CloneWorkRAM
				return this->_memory_accessor.Write(CloneWorkRAM::START_ADDRESS + this->RelativeAddress(absolute_address), data);
			}
		}

		return true;
	}

private:
	friend class DeviceManager;
};
} // Core

#endif // __LR35902_MEMORY_DEVICE_WORK_RAM_H__
