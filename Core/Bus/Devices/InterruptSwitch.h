/**
 * @file		InterruptSwitch.h
 * @author		Noam Rodrik
 * @description LR35902 main interrupt switch header.
 */
#ifndef __LR35902_MEMORY_DEVICE_INTERRUPT_SWITCH_H__
#define __LR35902_MEMORY_DEVICE_INTERRUPT_SWITCH_H__

#include <Core/CPU/Interrupts/Registers/InterruptEnable.h>
#include <API/Memory/Device/IMemoryDeviceAccess.h>
#include <API/Memory/Device/IMemoryDevice.h>
#include <API/Memory/Memory.h>
#include <API/Definitions.h>

namespace Core
{
class InterruptSwitch : public API::IMemoryDevice
{
public:
	constexpr InterruptSwitch(API::IMemoryDeviceAccess& memory_accessor) : API::IMemoryDevice{START_ADDRESS, END_ADDRESS, memory_accessor }, m_memory{}
	{
		// Default value for the interrupt flag
		this->m_memory[InterruptEnable::INTERRUPT_ENABLE_ADDRESS - START_ADDRESS] =
					   InterruptEnable::INTERRUPT_ENABLE_DEFAULT_VALUE;
	}

	virtual bool Read(const API::address_t absolute_address, API::data_t& result) const override
	{
		result = this->m_memory[absolute_address - START_ADDRESS];
		return true;
	}

	virtual void Write(const API::address_t absolute_address, const API::data_t data) override
	{
		this->m_memory[absolute_address - START_ADDRESS] = data;
	}

public:
	static constexpr uint16_t START_ADDRESS = 0xFFFF;
	static constexpr uint16_t END_ADDRESS = 0xFFFF;
	static constexpr size_t   SIZE = END_ADDRESS - START_ADDRESS + 1;

protected:
	virtual uint8_t* GetMemoryPointer() override { return this->m_memory.GetMemoryPointer(); }

private:
	API::Memory<SIZE> m_memory;

private:
	friend class DeviceManager;
};
} // Core

#endif // __LR35902_MEMORY_DEVICE_INTERRUPT_SWITCH_H__
