/**
 * @file		DeviceManagerBase.h
 * @author		Noam Rodrik
 * @description Defines the basic device manager base for the bus.
 */
#ifndef __DEVICE_MANAGER_BASE_H__
#define __DEVICE_MANAGER_BASE_H__

#include <Core/API/Memory/Device/MemoryDeviceOperations.h>
#include <Core/API/Memory/MemoryBase.h>
#include <Core/API/Definitions.h>
#include <Tools/not_null.h>
#include <stdint.h>

namespace Core
{
class DeviceManagerBase : public IMemoryDeviceAccess
{
public:
	constexpr DeviceManagerBase() : m_devices{}, m_last_added_device{0}
	{
		this->ClearDevices();
	}

	virtual void Write(const address_t data, const address_t absolute_address) override
	{
		for (const gsl::not_null<MemoryDeviceBase*> device : this->m_devices)
		{
			if (AddressInRange(absolute_address, device))
			{
				return device->Write(data, absolute_address);
			}
		}

		STOP_RUNNING("Didn't write anything to the devices.");
	}

	virtual bool Read(const address_t absolute_address, address_t& result) const override
	{
		for (const gsl::not_null<MemoryDeviceBase*> device : this->m_devices)
		{
			if (AddressInRange(absolute_address, device))
			{
				return device->Read(absolute_address, result);
			}
		}

		STOP_RUNNING("Failed reading from device -> The address is illegal and not in range!");
		return false;
	}

	virtual void Write(const data_t data, const address_t absolute_address) override
	{
		for (const gsl::not_null<MemoryDeviceBase*> device : this->m_devices)
		{
			if (AddressInRange(absolute_address, device))
			{
				return device->Write(data, absolute_address);
			}
		}
		
		STOP_RUNNING("Didn't write anything to the devices.");
	}

	virtual bool Read(const address_t absolute_address, data_t& result) const override
	{
		for (const gsl::not_null<MemoryDeviceBase*> device : this->m_devices)
		{
			if (AddressInRange(absolute_address, device))
			{
				return device->Read(absolute_address, result);
			}
		}

		STOP_RUNNING("Failed reading from device -> The address is illegal and not in range!");
		return false;
	}

	constexpr bool RegisterDevice(gsl::not_null<MemoryDeviceBase*> new_device)
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

	MemoryDeviceBase* GetDeviceAtAddress(const address_t absolute_address)
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

protected:
	constexpr void ClearDevices()
	{
		for (auto* device : this->m_devices)
		{
			delete device;
			device = nullptr;
		}
	}

protected:
	std::array<MemoryDeviceBase*, DEVICES_ON_BUS> m_devices;
	uint32_t m_last_added_device;
};
} // Core

#endif // __DEVICE_MANAGER_BASE_H__