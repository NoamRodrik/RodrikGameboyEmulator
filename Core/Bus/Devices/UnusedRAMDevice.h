/**
 * @file		UnusedRAMDevice.h
 * @author		Noam Rodrik
 * @description LR35902 main unused RAM device header.
 */
#ifndef __LR35902_MEMORY_DEVICE_UNUSED_RAM_H__
#define __LR35902_MEMORY_DEVICE_UNUSED_RAM_H__

#include <Core/Bus/MemoryDevice.h>
#include <API/Definitions.h>

namespace Core
{
class [[nodiscard]] UnusedRAMDevice : public MemoryDevice<0xFEA0, 0xFEFF>
{
public:
	UnusedRAMDevice(API::IMemoryDeviceAccess& memory_accessor) : MemoryDevice{memory_accessor} {}
	virtual ~UnusedRAMDevice() override = default;

private:
	friend class DeviceManager;
};
} // Core

#endif // __LR35902_MEMORY_DEVICE_OAM_RAM_H__