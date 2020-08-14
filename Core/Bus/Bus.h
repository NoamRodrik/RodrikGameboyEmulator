/**
 * @file		Bus.h
 * @author		Noam Rodrik
 * @description LR35902 main bus definition header.
 */
#ifndef __LR35902_BUS_H__
#define __LR35902_BUS_H__

#include <API/Memory/Device/IMemoryDeviceAccess.h>
#include <Core/Bus/DeviceManager/DeviceManager.h>

namespace Core
{
class Bus : public API::IMemoryDeviceAccess
{
public:
	Bus() = default;
	virtual ~Bus() = default;

public:
	virtual void Write(const API::address_t absolute_address, const API::data_t data) override
	{
		this->m_device_manager.Write(absolute_address, data);
	}

	virtual bool Read(const API::address_t absolute_address, API::data_t& result) const override
	{
		return this->m_device_manager.Read(absolute_address, result);
	}

	API::IMemoryDevice* GetDeviceAtAddress(const API::address_t absolute_address)
	{
		return this->m_device_manager.GetDeviceAtAddress(absolute_address);
	}

private:
	DeviceManager m_device_manager;

private:
	friend class Timer;
};
} // Core

#endif // __LR35902_BUS_H__
