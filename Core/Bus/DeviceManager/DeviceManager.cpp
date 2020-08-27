/**
 * @file		DeviceManager.cpp
 * @author		Noam Rodrik
 * @description LR35902's device manager implementation.
 */

#include "DeviceManager.h"
#include <Core/Bus/Devices/InterruptSwitch.h>
#include <Core/Bus/Devices/CartridgeRAM.h>
#include <Core/Bus/Devices/CloneWorkRAM.h>
#include <Core/Bus/Devices/ExternalRAM.h>
#include <Core/Bus/Devices/UnusedRAM.h>
#include <Core/Bus/Devices/VideoRAM.h>
#include <Core/Bus/Devices/HighRAM.h>
#include <Core/Bus/Devices/WorkRAM.h>
#include <Core/Bus/Devices/OAMRAM.h>
#include <Core/Bus/Devices/IORAM.h>
#include <API/Definitions.h>
#include <Tools/Tools.h>

using namespace API;

namespace Core
{
DeviceManager::DeviceManager() : m_devices{}, m_last_added_device{0}
{
	this->StartDevices();
}

DeviceManager::~DeviceManager()
{
	this->ClearDevices();
}

void DeviceManager::SetMemoryBankController(std::unique_ptr<API::IMemoryBankController> memory_bank_controller)
{
	this->m_mbc_controller = std::move(memory_bank_controller);

	// Loading cartridge.
	if (this->m_mbc_controller != nullptr)
	{
		this->m_mbc_controller->LoadMBC();
	}
}

void DeviceManager::StartDevices()
{
	static_assert(CartridgeRAM::SIZE + VideoRAM::SIZE + ExternalRAM::SIZE + WorkRAM::SIZE + CloneWorkRAM::SIZE +
				  OAMRAM::SIZE + UnusedRAM::SIZE + IORAM::SIZE + HighRAM::SIZE + InterruptSwitch::SIZE == MEMORY_SIZE);

	static gsl::not_null<IMemoryDevice*> devices_array[] = {new CartridgeRAM(*this),
															new VideoRAM(*this),
															new ExternalRAM(*this),
															new WorkRAM(*this),
															new CloneWorkRAM(*this),
															new OAMRAM(*this),
															new UnusedRAM(*this),
															new IORAM(*this),
															new HighRAM(*this),
															new InterruptSwitch(*this)};

	for (IMemoryDevice* device : devices_array)
	{
		SANITY(this->RegisterDevice(device), "Failed registering device to the device manager!");
	}
}

bool DeviceManager::Write(const address_t absolute_address, const API::data_t data)
{
	// If the MBC Controller decides to overwrite, no need to call devices.
	if (this->m_mbc_controller != nullptr &&
		this->m_mbc_controller->Write(absolute_address, data))
	{
		// Over-written.
		return true;
	}

	for (const gsl::not_null<IMemoryDevice*> device : this->m_devices)
	{
		if (AddressInRange(absolute_address, device))
		{
			return device->Write(absolute_address, data);
		}
	}

	LOG("Didn't write anything to the devices.");
	return false;
}

bool DeviceManager::Read(const address_t absolute_address, API::data_t& result) const
{
	// If the MBC Controller decides to overwrite, no need to call devices.
	if (this->m_mbc_controller != nullptr &&
		this->m_mbc_controller->Read(absolute_address, result))
	{
		// Over-written.
		return true;
	}

	for (const gsl::not_null<IMemoryDevice*> device : this->m_devices)
	{
		if (AddressInRange(absolute_address, device))
		{
			return device->Read(absolute_address, result);
		}
	}

	LOG("Failed reading from device -> The address is illegal and not in range!");
	return false;
}

IMemoryDevice* DeviceManager::GetDeviceAtAddress(const address_t absolute_address)
{
	for (auto* device : this->m_devices)
	{
		if (AddressInRange(absolute_address, device))
		{
			return device;
		}
	}

	return nullptr;
}
}