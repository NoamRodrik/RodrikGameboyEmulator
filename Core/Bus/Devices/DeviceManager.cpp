#include "DeviceManager.h"
#include <Core/Memory/Device/InterruptSwitch.h>
#include <Core/Memory/Device/CartridgeRAM.h>
#include <Core/Memory/Device/CloneWorkRAM.h>
#include <Core/Memory/Device/ExternalRAM.h>
#include <Core/Memory/Device/UnusedRAM.h>
#include <Core/Memory/Device/VideoRAM.h>
#include <Core/Memory/Device/HighRAM.h>
#include <Core/Memory/Device/WorkRAM.h>
#include <Core/Memory/Device/OAMRAM.h>
#include <Core/Memory/Device/IORAM.h>
#include <Core/LR35902/Definitions.h>
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