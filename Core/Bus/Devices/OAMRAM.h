/**
 * @file		OAMRAM.h
 * @author		Noam Rodrik
 * @description LR35902 main OAM RAM device header.
 */
#ifndef __LR35902_MEMORY_DEVICE_OAM_RAM_H__
#define __LR35902_MEMORY_DEVICE_OAM_RAM_H__

#include <Core/Bus/RAMDevice.h>
#include <API/Definitions.h>

namespace Core
{
class OAMRAM : public RAMDevice<0xFE00, 0xFE9F>
{
public:
	OAMRAM(API::IMemoryDeviceAccess& memory_accessor) : RAMDevice{memory_accessor} {}
	virtual ~OAMRAM() override = default;

private:
	friend class DeviceManager;
};
} // Core

#endif // __LR35902_MEMORY_DEVICE_OAM_RAM_H__
