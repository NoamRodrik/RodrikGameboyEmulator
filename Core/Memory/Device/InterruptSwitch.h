/**
 * @file		InterruptSwitch.h
 * @author		Noam Rodrik
 * @description LR35902 main interrupt switch header.
 */
#ifndef __LR35902_MEMORY_DEVICE_INTERRUPT_SWITCH_H__
#define __LR35902_MEMORY_DEVICE_INTERRUPT_SWITCH_H__

#include <Core/Interrupts/Registers/InterruptEnable.h>
#include <Core/API/Memory/Device/MemoryDeviceBase.h>
#include <Core/API/Definitions.h>
#include <Core/Memory/Memory.h>

namespace Core
{
class DeviceManagerBase;
} // Core

namespace Core
{
class InterruptSwitch : public MemoryDeviceBase
{
public:
	constexpr InterruptSwitch(DeviceManagerBase& device_manager) : MemoryDeviceBase{START_ADDRESS, END_ADDRESS, device_manager}, m_memory{}
	{
		// Default value for the interrupt flag
		this->m_memory[InterruptEnable::INTERRUPT_ENABLE_ADDRESS - START_ADDRESS] =
			InterruptEnable::INTERRUPT_ENABLE_DEFAULT_VALUE;
	}

	virtual bool Read(const address_t absolute_address, data_t& result) const override
	{
		result = this->m_memory[absolute_address - START_ADDRESS];
		return true;
	}

	virtual void Write(const address_t absolute_address, const data_t data) override
	{
		this->m_memory[absolute_address - START_ADDRESS] = data;
	}

	virtual bool Read(const address_t absolute_address, address_t& result) const override
	{
		STOP_RUNNING("Invalid command");
		return false;
	}

	virtual void Write(const address_t absolute_address, const address_t data) override
	{
		STOP_RUNNING("Invalid command");
	}

public:
	static constexpr uint16_t START_ADDRESS = 0xFFFF;
	static constexpr uint16_t END_ADDRESS = 0xFFFF;
	static constexpr size_t   SIZE = END_ADDRESS - START_ADDRESS + 1;

protected:
	virtual uint8_t* GetMemoryPointer() override { return this->m_memory.GetMemoryPointer(); }

private:
	Memory<SIZE> m_memory;

private:
	friend class DeviceManager;
};
} // Core

#endif // __LR35902_MEMORY_DEVICE_INTERRUPT_SWITCH_H__
