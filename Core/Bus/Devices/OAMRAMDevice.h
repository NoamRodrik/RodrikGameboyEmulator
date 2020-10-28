/**
 * @file		OAMRAMDevice.h
 * @author		Noam Rodrik
 * @description LR35902 main OAM RAM device header.
 */
#ifndef __LR35902_MEMORY_DEVICE_OAM_RAM_H__
#define __LR35902_MEMORY_DEVICE_OAM_RAM_H__

#include <Core/Bus/MemoryDevice.h>
#include <API/Definitions.h>

namespace Core
{
class [[nodiscard]] OAMRAMDevice : public MemoryDevice<0xFE00, 0xFE9F>
{
public:
	OAMRAMDevice(API::IMemoryDeviceAccess& memory_accessor) : MemoryDevice{memory_accessor} {}
	virtual ~OAMRAMDevice() override = default;

private:
	friend class DeviceManager;
};
} // Core

#endif // __LR35902_MEMORY_DEVICE_OAM_RAM_H__
