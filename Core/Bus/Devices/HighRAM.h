/**
 * @file		HighRAM.h
 * @author		Noam Rodrik
 * @description LR35902 main high RAM device header.
 */
#ifndef __LR35902_MEMORY_DEVICE_HIGH_RAM_H__
#define __LR35902_MEMORY_DEVICE_HIGH_RAM_H__

#include <Core/Bus/RAMDevice.h>
#include <API/Definitions.h>

namespace Core
{
class [[nodiscard]] HighRAM : public RAMDevice<0xFF80, 0xFFFE>
{
public:
	HighRAM(API::IMemoryDeviceAccess& memory_accessor) : RAMDevice{memory_accessor} {}
	virtual ~HighRAM() override = default;

private:
	friend class DeviceManager;
};
} // Core

#endif // __LR35902_MEMORY_DEVICE_HIGH_RAM_H__
