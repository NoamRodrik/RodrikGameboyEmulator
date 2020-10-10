/**
 * @file		ExternalRAM.h
 * @author		Noam Rodrik
 * @description LR35902 main external RAM device header.
 */
#ifndef __LR35902_MEMORY_DEVICE_EXTERNAL_RAM_H__
#define __LR35902_MEMORY_DEVICE_EXTERNAL_RAM_H__

#include <Core/Bus/RAMDevice.h>
#include <API/Definitions.h>

namespace Core
{
class [[nodiscard]] ExternalRAM : public RAMDevice<0xA000, 0xBFFF>
{
public:
	ExternalRAM(API::IMemoryDeviceAccess& memory_accessor) : RAMDevice{memory_accessor} {}
	virtual ~ExternalRAM() override = default;

private:
	friend class DeviceManager;
};
} // Core

#endif // __LR35902_MEMORY_DEVICE_EXTERNAL_RAM_H__
