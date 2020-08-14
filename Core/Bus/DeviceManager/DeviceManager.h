/**
 * @file		DeviceManager.h
 * @author		Noam Rodrik
 * @description Defines the basic device manager base for the bus.
 */
#ifndef __DEVICE_MANAGER_H__
#define __DEVICE_MANAGER_H__

#include <API/Memory/Device/MemoryDeviceOperations.h>
#include <API/Memory/Device/IMemoryDeviceAccess.h>
#include <Contrib/GSL/not_null.h>
#include <API/Definitions.h>
#include <cstdint>

namespace Core
{
class DeviceManager : public API::IMemoryDeviceAccess
{
public:
	DeviceManager();
	virtual ~DeviceManager();

public:
	virtual void Write(const API::address_t absolute_address, const API::data_t data) override;
	virtual bool Read(const API::address_t absolute_address, API::data_t& result) const override;

	API::IMemoryDevice* GetDeviceAtAddress(const API::address_t absolute_address);

protected:
	constexpr bool RegisterDevice(gsl::not_null<API::IMemoryDevice*> new_device)
	{
		SANITY(this->m_last_added_device < this->m_devices.size(), "Added too many elements, overflow!");

		for (size_t device_index = 0; device_index < this->m_last_added_device; ++device_index)
		{
			SANITY(this->m_devices[device_index] != nullptr, "Found a null device?");
			RET_FALSE_IF_FAIL(!MemoryOverlap(this->m_devices[device_index], new_device),
				"Two devices are sharing the same memory addresses, error!");
		}

		// Adding to the pool of devices.
		this->m_devices[this->m_last_added_device++] = new_device;
		return true;
	}

	constexpr void ClearDevices()
	{
		for (auto* device : this->m_devices)
		{
			delete device;
			device = nullptr;
		}
	}

private:
	void StartDevices();

protected:
	std::array<API::IMemoryDevice*, API::DEVICES_ON_BUS> m_devices;
	uint32_t m_last_added_device;
};
} // Core

#endif // __DEVICE_MANAGER_H__