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

void DeviceManager::Write(const address_t absolute_address, const API::data_t data)
{
	for (const gsl::not_null<IMemoryDevice*> device : this->m_devices)
	{
		if (AddressInRange(absolute_address, device))
		{
			return device->Write(absolute_address, data);
		}
	}

	STOP_RUNNING("Didn't write anything to the devices.");
}

bool DeviceManager::Read(const address_t absolute_address, API::data_t& result) const
{
	for (const gsl::not_null<IMemoryDevice*> device : this->m_devices)
	{
		if (AddressInRange(absolute_address, device))
		{
			return device->Read(absolute_address, result);
		}
	}

	STOP_RUNNING("Failed reading from device -> The address is illegal and not in range!");
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