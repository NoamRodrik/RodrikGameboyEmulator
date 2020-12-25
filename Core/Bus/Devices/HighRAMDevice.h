/**
 * @file		HighRAMDevice.h
 * @author		Noam Rodrik
 * @description LR35902 main high RAM device header.
 */
#ifndef __LR35902_MEMORY_DEVICE_HIGH_RAM_H__
#define __LR35902_MEMORY_DEVICE_HIGH_RAM_H__

#include <Core/Bus/MemoryDevice.h>
#include <API/Definitions.h>

namespace Core
{
class [[nodiscard]] HighRAMDevice : public MemoryDevice<0xFF80, 0xFFFE>
{
public:
	HighRAMDevice(API::IMemoryDeviceAccess& memory_accessor) : MemoryDevice{memory_accessor} {}
	virtual ~HighRAMDevice() override = default;

private:
	friend class DeviceManager;
};
} // Core

#endif // __LR35902_MEMORY_DEVICE_HIGH_RAM_H__
