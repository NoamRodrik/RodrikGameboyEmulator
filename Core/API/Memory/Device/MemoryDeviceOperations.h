/**
 * @file		MemoryDeviceOperations.h
 * @author		Noam Rodrik
 * @description Defines basic actions for memory device base functionality.
 */
#ifndef __MEMORY_DEVICE_OPERATIONS_H__
#define __MEMORY_DEVICE_OPERATIONS_H__

#include <Core/API/Definitions.h>
#include "MemoryDeviceBase.h"
#include <Tools/not_null.h>
#include <algorithm>

namespace Core
{
	/**
	 * Checks if two ranges, [start_1, end_1], [start_2, end_2], overlap. Then it returns true.
	 */
	constexpr bool MemoryOverlap(const gsl::not_null<MemoryDeviceBase*> first_device, const gsl::not_null<MemoryDeviceBase*> second_device)
	{
		return std::max<uint32_t>(first_device->GetStartAddress(), second_device->GetStartAddress()) <=
			   std::min<uint32_t>(first_device->GetEndAddress(), second_device->GetEndAddress());
	}
	
	/**
	 * Checks if an address is in range of a device.
	 */
	constexpr bool AddressInRange(const address_t address, const gsl::not_null<MemoryDeviceBase*> device)
	{
		return address >= device->GetStartAddress() && address <= device->GetEndAddress();
	}
} // Core

#endif // __MEMORY_DEVICE_OPERATIONS_H__