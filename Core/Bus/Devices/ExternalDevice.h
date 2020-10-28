/**
 * @file		ExternalDevice.h
 * @author		Noam Rodrik
 * @description LR35902 main external RAM device header.
 */
#ifndef __LR35902_MEMORY_DEVICE_EXTERNAL_RAM_H__
#define __LR35902_MEMORY_DEVICE_EXTERNAL_RAM_H__

#include <Core/Bus/MemoryDevice.h>
#include <API/Definitions.h>

namespace Core
{
class [[nodiscard]] ExternalDevice : public MemoryDevice<0xA000, 0xBFFF>
{
public:
	ExternalDevice(API::IMemoryDeviceAccess& memory_accessor) : MemoryDevice{memory_accessor} {}
	virtual ~ExternalDevice() override = default;

private:
	friend class DeviceManager;
};
} // Core

#endif // __LR35902_MEMORY_DEVICE_EXTERNAL_RAM_H__
