/**
 * @file		IRAMDevice.h
 * @author		Noam Rodrik
 * @description LR35902 main ram device interface.
 */
#ifndef __LR35902_IRAM_DEVICE_H__
#define __LR35902_IRAM_DEVICE_H__

#include <API/Memory/Device/IMemoryDevice.h>
#include <API/Definitions.h>

namespace Core
{
class IRAMDevice
{
public:
	virtual ~IRAMDevice() = default;

protected:
	/**
	 * If intercepted, the write will not occur.
	 */
	virtual bool Intercept(const API::address_t absolute_address, const API::data_t data) = 0;

private:
	friend class DeviceManager;
};
} // Core

#endif // __LR35902_IRAM_DEVICE_H__
