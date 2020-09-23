/**
 * @file		Video.h
 * @author		Noam Rodrik
 * @description LR35902 main video RAM device header.
 */
#ifndef __LR35902_MEMORY_DEVICE_VIDEO_RAM_H__
#define __LR35902_MEMORY_DEVICE_VIDEO_RAM_H__

#include <Core/Bus/RAMDevice.h>
#include <API/Definitions.h>

namespace Core
{
class VideoRAM : public RAMDevice<0x8000, 0x9FFF>
{
public:
	VideoRAM(API::IMemoryDeviceAccess& memory_accessor) : RAMDevice{memory_accessor} {}
	virtual ~VideoRAM() override = default;

private:
	friend class DeviceManager;
};
} // Core

#endif // __LR35902_MEMORY_DEVICE_VIDEO_RAM_H__
