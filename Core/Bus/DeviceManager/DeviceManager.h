/**
 * @file		DeviceManager.h
 * @author		Noam Rodrik
 * @description Defines the basic device manager base for the bus.
 */
#ifndef __DEVICE_MANAGER_H__
#define __DEVICE_MANAGER_H__

#include <API/Memory/Device/MemoryDeviceOperations.h>
#include <API/Memory/Device/IMemoryDeviceAccess.h>
#include <API/Cartridge/IMemoryBankController.h>
#include <Contrib/GSL/not_null.h>
#include <API/Definitions.h>
#include <cstdint>

namespace Core
{
class DeviceManager : public API::IMemoryDeviceAccess
{
public:
	DeviceManager();
	virtual ~DeviceManager() = default;

public:
	virtual bool Write(const API::address_t absolute_address, const API::data_t data) override;
	virtual bool Read(const API::address_t absolute_address, API::data_t& result) const override;
	virtual bool WriteDirectly(const API::address_t absolute_address, const API::data_t data) override;

	API::IMemoryDevice* GetDeviceAtAddress(const API::address_t absolute_address);

	void SetMemoryBankController(std::unique_ptr<API::IMemoryBankController> memory_bank_controller);

protected:
	bool RegisterDevice(gsl::unique_ptr<API::IMemoryDevice> new_device)
	{
		SANITY(this->_last_added_device < this->_devices.size(), "Added too many elements, overflow!");

		for (size_t device_index = 0; device_index < this->_last_added_device; ++device_index)
		{
			SANITY(this->_devices[device_index] != nullptr, "Found a null device?");
			RET_FALSE_IF_FAIL(!MemoryOverlap(this->_devices[device_index].get(), new_device.get()),
				"Two devices are sharing the same memory addresses, error!");
		}

		// Adding to the pool of devices.
		this->_devices[this->_last_added_device++] = std::move(new_device);
		return true;
	}

private:
	void StartDevices();

protected:
	std::array<std::unique_ptr<API::IMemoryDevice>, API::DEVICES_ON_BUS> _devices{};
	std::unique_ptr<API::IMemoryBankController> _mbc_controller{nullptr};
	uint32_t _last_added_device{0};

private:
	friend class MainPixelEngine;
};
} // Core

#endif // __DEVICE_MANAGER_H__