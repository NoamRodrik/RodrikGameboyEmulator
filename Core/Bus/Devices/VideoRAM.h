/**
 * @file		Video.h
 * @author		Noam Rodrik
 * @description LR35902 main video RAM device header.
 */
#ifndef __LR35902_MEMORY_DEVICE_VIDEO_RAM_H__
#define __LR35902_MEMORY_DEVICE_VIDEO_RAM_H__

#include <API/Memory/Device/IMemoryDeviceAccess.h>
#include <API/Memory/Device/IMemoryDevice.h>
#include <API/Memory/Memory.h>
#include <API/Definitions.h>

namespace Core
{
class VideoRAM : public API::IMemoryDevice
{
public:
	constexpr VideoRAM(API::IMemoryDeviceAccess& memory_accessor) : API::IMemoryDevice{START_ADDRESS, END_ADDRESS, memory_accessor }, m_memory{} {}

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
	static constexpr uint16_t START_ADDRESS = 0x8000;
	static constexpr uint16_t END_ADDRESS = 0x9FFF;
	static constexpr size_t   SIZE = END_ADDRESS - START_ADDRESS + 1;

protected:
	virtual uint8_t* GetMemoryPointer() override { return this->m_memory.GetMemoryPointer(); }

private:
	API::Memory<SIZE> m_memory;

private:
	friend class DeviceManager;
};
} // Core

#endif // __LR35902_MEMORY_DEVICE_VIDEO_RAM_H__
