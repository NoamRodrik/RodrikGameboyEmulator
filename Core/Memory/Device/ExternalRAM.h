/**
 * @file		ExternalRAM.h
 * @author		Noam Rodrik
 * @description LR35902 main external RAM device header.
 */
#ifndef __LR35902_MEMORY_DEVICE_EXTERNAL_RAM_H__
#define __LR35902_MEMORY_DEVICE_EXTERNAL_RAM_H__

#include <Core/API/Memory/Device/MemoryDeviceBase.h>
#include <Core/Memory/Memory.h>
#include <Core/API/Definitions.h>

namespace Core
{
class DeviceManagerBase;
} // Core

namespace Core
{
class ExternalRAM : public MemoryDeviceBase
{
public:
	constexpr ExternalRAM(DeviceManagerBase& device_manager) : MemoryDeviceBase{START_ADDRESS, END_ADDRESS, device_manager}, m_memory{} {}

	virtual bool Read(const address_t absolute_address, data_t& result) const override
	{
		result = this->m_memory[absolute_address - START_ADDRESS];
		return true;
	}

	virtual void Write(const address_t absolute_address, const data_t data) override
	{
		this->m_memory[absolute_address - START_ADDRESS] = data;
	}

public:
	static constexpr uint16_t START_ADDRESS = 0xA000;
	static constexpr uint16_t END_ADDRESS = 0xBFFF;
	static constexpr size_t   SIZE = END_ADDRESS - START_ADDRESS + 1;

protected:
	virtual uint8_t* GetMemoryPointer() override { return this->m_memory.GetMemoryPointer(); }

private:
	Memory<SIZE> m_memory;

private:
	friend class DeviceManager;
};
} // Core

#endif // __LR35902_MEMORY_DEVICE_EXTERNAL_RAM_H__
