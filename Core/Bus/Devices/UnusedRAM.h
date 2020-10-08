/**
 * @file		UnusedRAM.h
 * @author		Noam Rodrik
 * @description LR35902 main unused RAM device header.
 */
#ifndef __LR35902_MEMORY_DEVICE_UNUSED_RAM_H__
#define __LR35902_MEMORY_DEVICE_UNUSED_RAM_H__

#include <Core/Bus/RAMDevice.h>
#include <API/Definitions.h>

namespace Core
{
class [[nodiscard]] UnusedRAM : public RAMDevice<0xFEA0, 0xFEFF>
{
public:
	UnusedRAM(API::IMemoryDeviceAccess& memory_accessor) : RAMDevice{memory_accessor} {}
	virtual ~UnusedRAM() override = default;

private:
	friend class DeviceManager;
};
} // Core

#endif // __LR35902_MEMORY_DEVICE_OAM_RAM_H__