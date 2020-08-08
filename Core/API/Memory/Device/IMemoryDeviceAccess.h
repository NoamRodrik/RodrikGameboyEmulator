/**
 * @file		IMemoryDeviceAccess.h
 * @author		Noam Rodrik
 * @description Defines basic actions for memory access device objects.
 */
#ifndef __IMEMORY_DEVICE_ACCESS_H__
#define __IMEMORY_DEVICE_ACCESS_H__

#include <Core/API/Definitions.h>

namespace Core
{
class IMemoryDeviceAccess
{
public:
	virtual ~IMemoryDeviceAccess() = default;
	
public:
	virtual bool Read(const address_t absolute_address, data_t& result) const = 0;
	virtual void Write(const address_t absolute_address, const data_t data) = 0;
};
} // Core

#endif // __IMEMORY_DEVICE_ACCESS_H__