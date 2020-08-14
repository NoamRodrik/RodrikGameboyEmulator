/**
 * @file		DeviceManager.h
 * @author		Noam Rodrik
 * @description LR35902 main bus definition header.
 */
#ifndef __LR35902_DEVICE_MANAGER_H__
#define __LR35902_DEVICE_MANAGER_H__

#include <Core/Bus/DeviceManager/DeviceManagerBase.h>

namespace Core
{
class DeviceManager : public DeviceManagerBase
{
public:
	DeviceManager();
	virtual ~DeviceManager();

private:
	void StartDevices();
};
} // Core

#endif // __LR35902_DEVICE_MANAGER_H__
