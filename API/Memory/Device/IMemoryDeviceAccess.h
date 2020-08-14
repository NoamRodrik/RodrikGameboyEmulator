/**
 * @file		IMemoryDeviceAccess.h
 * @author		Noam Rodrik
 * @description Defines basic actions for memory access device objects.
 */
#ifndef __IMEMORY_DEVICE_ACCESS_H__
#define __IMEMORY_DEVICE_ACCESS_H__

#include <API/Definitions.h>

namespace API
{
class IMemoryDeviceAccess
{
public:
	virtual ~IMemoryDeviceAccess() = default;
	
public:
	virtual bool Read(const address_t absolute_address, data_t& result) const = 0;
	virtual void Write(const address_t absolute_address, const data_t data) = 0;
};
} // API

#endif // __IMEMORY_DEVICE_ACCESS_H__