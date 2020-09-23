/**
 * @file		CloneWorkRAM.h
 * @author		Noam Rodrik
 * @description LR35902 main clone work RAM device header.
 */
#ifndef __LR35902_MEMORY_DEVICE_CLONE_WORK_RAM_H__
#define __LR35902_MEMORY_DEVICE_CLONE_WORK_RAM_H__

#include <API/Memory/Device/IMemoryDeviceAccess.h>
#include <API/Memory/Device/IMemoryDevice.h>
#include <API/Memory/Memory.h>
#include <API/Definitions.h>

namespace Core
{
class WorkRAM;

class CloneWorkRAM : public API::IMemoryDevice
{
public:
	constexpr CloneWorkRAM(API::IMemoryDeviceAccess& m_memory_accessor) : API::IMemoryDevice{START_ADDRESS, END_ADDRESS, m_memory_accessor} {}

	virtual bool Read(const API::address_t absolute_address, API::data_t& result) const override
	{
		result = this->_memory[absolute_address - START_ADDRESS];
		return true;
	}

	virtual bool Write(const API::address_t absolute_address, const API::data_t data) override
	{
		this->_memory[GetFixedAddress(absolute_address)] = data;

		API::data_t work_ram_data{0};

		RET_FALSE_IF_FAIL(this->_memory_accessor.Read(0xC000 + GetFixedAddress(absolute_address), work_ram_data),
						  "Failed reading from WorkRAM");

		if (work_ram_data != data)
		{
			return this->_memory_accessor.Write(0xC000 + GetFixedAddress(absolute_address), work_ram_data);
		}

		return true;
	}

public:
	static constexpr API::address_t START_ADDRESS = 0xE000;
	static constexpr API::address_t END_ADDRESS = 0xFDFF;
	static constexpr size_t   SIZE = END_ADDRESS - START_ADDRESS + 1;

protected:
	virtual uint8_t* GetMemoryPointer() override { return this->_memory.GetMemoryPointer(); }

private:
	static constexpr API::address_t GetFixedAddress(const API::address_t address) { return address - START_ADDRESS; }

private:
	API::Memory<SIZE> _memory;

private:
	friend class DeviceManager;
};
} // Core

#endif // __LR35902_MEMORY_DEVICE_CLONE_WORK_RAM_H__
