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
#include <Core/API/Definitions.h>
#include <Tools/Tools.h>

namespace Core
{
DeviceManager::DeviceManager() : DeviceManagerBase{}
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

	static gsl::not_null<MemoryDeviceBase*> devices_array[] = {new CartridgeRAM(*this),
															   new VideoRAM(*this),
															   new ExternalRAM(*this),
															   new WorkRAM(*this),
															   new CloneWorkRAM(*this),
															   new OAMRAM(*this),
															   new UnusedRAM(*this),
															   new IORAM(*this),
															   new HighRAM(*this),
															   new InterruptSwitch(*this)};

	for (MemoryDeviceBase* device : devices_array)
	{
		SANITY(this->RegisterDevice(device), "Failed registering device to the device manager!");
	}
}
}