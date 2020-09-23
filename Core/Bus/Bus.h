/**
 * @file		Bus.h
 * @author		Noam Rodrik
 * @description LR35902 main bus definition header.
 */
#ifndef __LR35902_BUS_H__
#define __LR35902_BUS_H__

#include <API/Memory/Device/IMemoryDeviceAccess.h>
#include <Core/Bus/DeviceManager/DeviceManager.h>
#include <Core/Cartridge/Controller.h>

namespace Core
{
class Bus : public API::IMemoryDeviceAccess
{
public:
	Bus() = default;
	virtual ~Bus() = default;

public:
	virtual bool Write(const API::address_t absolute_address, const API::data_t data) override
	{
		return this->_device_manager.Write(absolute_address, data);
	}

	virtual bool Read(const API::address_t absolute_address, API::data_t& result) const override
	{
		return this->_device_manager.Read(absolute_address, result);
	}

	API::IMemoryDevice* GetDeviceAtAddress(const API::address_t absolute_address)
	{
		return this->_device_manager.GetDeviceAtAddress(absolute_address);
	}

	void SetMemoryBankController(std::shared_ptr<API::ILoader> loader)
	{
		this->_device_manager.SetMemoryBankController(std::make_unique<MBCController>(*this, loader));
	}

private:
	DeviceManager _device_manager{};

private:
	friend class Timer;
};
} // Core

#endif // __LR35902_BUS_H__
