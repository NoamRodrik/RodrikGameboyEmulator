/**
 * @file		HighRAM.h
 * @author		Noam Rodrik
 * @description LR35902 main high RAM device header.
 */
#ifndef __LR35902_MEMORY_DEVICE_HIGH_RAM_H__
#define __LR35902_MEMORY_DEVICE_HIGH_RAM_H__

#include <Core/API/Memory/Device/MemoryDeviceBase.h>
#include <Core/Memory/Memory.h>
#include <Core/API/Definitions.h>

namespace Core
{
class DeviceManagerBase;
} // Core

namespace Core
{
class HighRAM : public MemoryDeviceBase
{
public:
	constexpr HighRAM(DeviceManagerBase& device_manager) : MemoryDeviceBase{START_ADDRESS, END_ADDRESS, device_manager}, m_memory{} {}

	virtual bool Read(const address_t absolute_address, data_t& result) const override
	{
		result = this->m_memory[GetFixedAddress(absolute_address)];
		return true;
	}

	virtual void Write(const address_t absolute_address, const data_t data) override
	{
		this->m_memory[GetFixedAddress(absolute_address)] = data;
	}

public:
	static constexpr uint16_t START_ADDRESS = 0xFF80;
	static constexpr uint16_t END_ADDRESS = 0xFFFE;
	static constexpr size_t   SIZE = END_ADDRESS - START_ADDRESS + 1;

protected:
	virtual uint8_t* GetMemoryPointer() override { return this->m_memory.GetMemoryPointer(); }

private:
	static constexpr address_t GetFixedAddress(const address_t address) { return address - START_ADDRESS; }

private:
	Memory<SIZE> m_memory;

private:
	friend class DeviceManager;
};
} // Core

#endif // __LR35902_MEMORY_DEVICE_HIGH_RAM_H__
