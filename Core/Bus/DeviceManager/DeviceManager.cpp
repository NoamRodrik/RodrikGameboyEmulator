/**
 * @file		DeviceManager.cpp
 * @author		Noam Rodrik
 * @description LR35902's device manager implementation.
 */

#include "DeviceManager.h"
#include <Core/Bus/Devices/InterruptSwitch.h>
#include <Core/Bus/Devices/CartridgeDevice.h>
#include <Core/Bus/Devices/CloneWorkRAMDevice.h>
#include <Core/Bus/Devices/ExternalDevice.h>
#include <Core/Bus/Devices/UnusedRAMDevice.h>
#include <Core/Bus/Devices/VideoRAMDevice.h>
#include <Core/Bus/Devices/HighRAMDevice.h>
#include <Core/Bus/Devices/WorkRAMDevice.h>
#include <Core/Bus/Devices/OAMRAMDevice.h>
#include <Core/Bus/Devices/IODevice.h>
#include <API/Definitions.h>
#include <Tools/Tools.h>

using namespace API;

namespace Core
{
DeviceManager::DeviceManager()
{
	this->StartDevices();
}

void DeviceManager::SetMemoryBankController(std::unique_ptr<API::IMemoryBankController> memory_bank_controller)
{
	this->_mbc_controller = std::move(memory_bank_controller);

	// Loading cartridge.
	if (this->_mbc_controller != nullptr)
	{
		this->_mbc_controller->LoadMBC();
	}
}

void DeviceManager::StartDevices()
{
	static constexpr auto DEVICE_MEMORY_SIZE = CartridgeDevice::SIZE + VideoRAMDevice::SIZE + ExternalDevice::SIZE + WorkRAMDevice::SIZE + CloneWorkRAMDevice::SIZE +
		OAMRAMDevice::SIZE + UnusedRAMDevice::SIZE + IODevice::SIZE + HighRAMDevice::SIZE + InterruptSwitch::SIZE;

	static_assert(DEVICE_MEMORY_SIZE == MEMORY_SIZE, "Size of memory is invalid.");

	std::unique_ptr<IMemoryDevice> devices_array[] = {std::make_unique<CartridgeDevice>(*this),
	 											      std::make_unique<VideoRAMDevice>(*this),
	 											      std::make_unique<ExternalDevice>(*this),
	 											      std::make_unique<WorkRAMDevice>(*this),
	 											      std::make_unique<CloneWorkRAMDevice>(*this),
	 											      std::make_unique<OAMRAMDevice>(*this),
	 											      std::make_unique<UnusedRAMDevice>(*this),
	 											      std::make_unique<IODevice>(*this),
	 											      std::make_unique<HighRAMDevice>(*this),
	 											      std::make_unique<InterruptSwitch>(*this)};

	for (auto&& device : devices_array)
	{
		SANITY(this->RegisterDevice(std::move(device)), "Failed registering device to the device manager!");
	}
}


bool DeviceManager::WriteDirectly(const API::address_t absolute_address, const API::data_t data)
{
	// If the MBC Controller decides to intercept, no need to call devices.
	if (this->_mbc_controller != nullptr &&
		this->_mbc_controller->WriteDirectly(absolute_address, data))
	{
		// Intercepted.
		return true;
	}

	for (auto&& device : this->_devices)
	{
		if (AddressInRange(absolute_address, device.get()))
		{
			return device->WriteDirectly(absolute_address, data);
		}
	}

	LOG("Didn't write anything to the devices.");
	return false;
}

bool DeviceManager::Write(const address_t absolute_address, const API::data_t data)
{
	// If the MBC Controller decides to intercept, no need to call devices.
	if (this->_mbc_controller != nullptr &&
		this->_mbc_controller->Write(absolute_address, data))
	{
		// Intercepted.
		return true;
	}

	for (auto&& device : this->_devices)
	{
		if (AddressInRange(absolute_address, device.get()))
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
	if (this->_mbc_controller != nullptr &&
		this->_mbc_controller->Read(absolute_address, result))
	{
		// Over-written.
		return true;
	}

	for (auto&& device : this->_devices)
	{
		if (AddressInRange(absolute_address, device.get()))
		{
			return device->Read(absolute_address, result);
		}
	}

	LOG("Failed reading from device -> The address is illegal and not in range!");
	return false;
}

IMemoryDevice* DeviceManager::GetDeviceAtAddress(const address_t absolute_address)
{
	for (auto&& device : this->_devices)
	{
		if (AddressInRange(absolute_address, device.get()))
		{
			return device.get();
		}
	}

	return nullptr;
}
}