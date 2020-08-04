/**
 * @file		Cartridge.h
 * @author		Noam Rodrik
 * @description LR35902 main cartridge RAM device header.
 */
#ifndef __LR35902_MEMORY_DEVICE_CARTRIDGE_RAM_H__
#define __LR35902_MEMORY_DEVICE_CARTRIDGE_RAM_H__

#include <Core/API/Memory/Device/MemoryDeviceBase.h>
#include <Core/Memory/Memory.h>
#include <Core/API/Definitions.h>
#include <Core/Loader/BinaryLoader.h>
#include <Core/LR35902/Boot.h>
#include <algorithm>

namespace Core
{
class DeviceManagerBase;
} // Core

namespace Core
{
class CartridgeRAM : public MemoryDeviceBase
{
public:
	CartridgeRAM(DeviceManagerBase& device_manager) : MemoryDeviceBase{START_ADDRESS, END_ADDRESS, device_manager}, m_memory{}
	{
		LoadDMGBoot();
	}

	virtual bool Read(const address_t absolute_address, data_t& result) const override
	{
		result = this->m_memory[absolute_address - START_ADDRESS];
		return true;
	}

	virtual void Write(const data_t data, const address_t absolute_address) override
	{
		this->m_memory[absolute_address - START_ADDRESS] = data;
	}

	virtual bool Read(const address_t absolute_address, address_t& result) const override
	{
		result = this->m_memory[absolute_address - START_ADDRESS] | (this->m_memory[absolute_address - START_ADDRESS + 1] << 8);
		return true;
	}

	virtual void Write(const address_t data, const address_t absolute_address) override
	{
		this->m_memory[absolute_address - START_ADDRESS] = data & 0x00FF;
		this->m_memory[absolute_address - START_ADDRESS + 1] = (data & 0xFF00 >> 8);
	}

public:
	static constexpr uint16_t START_ADDRESS = 0x0000;
	static constexpr uint16_t END_ADDRESS = 0x7FFF;
	static constexpr size_t   SIZE = END_ADDRESS - START_ADDRESS + 1;

protected:
	virtual uint8_t* GetMemoryPointer() override { return this->m_memory.GetMemoryPointer(); }

private:
	void LoadDMGBoot()
	{
#if SKIP_BOOT
		std::copy(SYSTEM_BOOT_CODE.data(),
				  SYSTEM_BOOT_CODE.data() + SYSTEM_BOOT_CODE.size(),
				  this->GetMemoryPointer());
#endif
	}

private:
	Memory<SIZE> m_memory;

private:
	friend class DeviceManager;
};
} // Core

#endif // __LR35902_MEMORY_DEVICE_CARTRIDGE_RAM_H__
