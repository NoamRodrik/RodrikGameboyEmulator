/**
 * @file		VideoRAMDevice.h
 * @author		Noam Rodrik
 * @description LR35902 main video RAM device header.
 */
#ifndef __LR35902_MEMORY_DEVICE_VIDEO_RAM_H__
#define __LR35902_MEMORY_DEVICE_VIDEO_RAM_H__

#include <Core/Bus/MemoryDevice.h>
#include <API/Definitions.h>

namespace Core
{
class [[nodiscard]] VideoRAMDevice : public MemoryDevice<0x8000, 0x9FFF>
{
public:
	VideoRAMDevice(API::IMemoryDeviceAccess& memory_accessor) : MemoryDevice{memory_accessor} {}
	virtual ~VideoRAMDevice() override = default;

private:
	friend class Fetcher;
	friend class DeviceManager;
};
} // Core

#endif // __LR35902_MEMORY_DEVICE_VIDEO_RAM_H__
