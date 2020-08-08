/**
 * @file		WorkRAM.h
 * @author		Noam Rodrik
 * @description LR35902 main work RAM device header.
 */
#ifndef __LR35902_MEMORY_DEVICE_WORK_RAM_H__
#define __LR35902_MEMORY_DEVICE_WORK_RAM_H__

#include <Core/Memory/Device/CloneWorkRAM.h>
#include <Core/API/Memory/Device/MemoryDeviceBase.h>
#include <Core/Memory/Memory.h>
#include <Core/API/Definitions.h>

namespace Core
{
class DeviceManagerBase;
} // Core

namespace Core
{
class WorkRAM : public MemoryDeviceBase
{
public:
	constexpr WorkRAM(DeviceManagerBase& device_manager) : MemoryDeviceBase{START_ADDRESS, END_ADDRESS, device_manager}, m_memory{} {}

	virtual bool Read(const address_t absolute_address, data_t& result) const override
	{
		result = this->m_memory[absolute_address - START_ADDRESS];
		return true;
	}

	virtual void Write(const address_t absolute_address, const data_t data) override
	{
		this->m_memory[absolute_address - START_ADDRESS] = data;

		data_t clone_data{0};

		SANITY(this->m_device_manager.Read(CloneWorkRAM::START_ADDRESS + absolute_address - START_ADDRESS, clone_data),
			"Failed reading from CloneWorkRAM");

		if (clone_data != data)
		{
			// Clones to the CloneWorkRAM
			this->m_device_manager.Write(CloneWorkRAM::START_ADDRESS + (absolute_address - START_ADDRESS), data);
		}
	}

public:
	static constexpr uint16_t START_ADDRESS = 0xC000;
	static constexpr uint16_t END_ADDRESS = 0xDFFF;
	static constexpr size_t   SIZE = END_ADDRESS - START_ADDRESS + 1;

protected:
	virtual uint8_t* GetMemoryPointer() override { return this->m_memory.GetMemoryPointer(); }

private:
	Memory<SIZE> m_memory;

private:
	friend class DeviceManager;
};
} // Core

#endif // __LR35902_MEMORY_DEVICE_WORK_RAM_H__
