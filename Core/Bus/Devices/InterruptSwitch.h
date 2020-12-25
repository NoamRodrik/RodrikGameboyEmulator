/**
 * @file		InterruptSwitch.h
 * @author		Noam Rodrik
 * @description LR35902 main interrupt switch header.
 */
#ifndef __LR35902_MEMORY_DEVICE_INTERRUPT_SWITCH_H__
#define __LR35902_MEMORY_DEVICE_INTERRUPT_SWITCH_H__

#include <Core/CPU/Interrupts/Registers/InterruptEnable.h>
#include <Core/Bus/MemoryDevice.h>
#include <API/Definitions.h>

namespace Core
{
class [[nodiscard]] InterruptSwitch : public MemoryDevice<0xFFFF, 0xFFFF>
{
public:
	InterruptSwitch(API::IMemoryDeviceAccess& memory_accessor) : MemoryDevice{memory_accessor}
	{
		SANITY(this->Write(InterruptEnable::MEMORY_ADDRESS, InterruptEnable::INTERRUPT_ENABLE_DEFAULT_VALUE),
			   "Failed setting interrupt switch");
	}

	virtual ~InterruptSwitch() override = default;

private:
	friend class DeviceManager;
};
} // Core

#endif // __LR35902_MEMORY_DEVICE_INTERRUPT_SWITCH_H__
