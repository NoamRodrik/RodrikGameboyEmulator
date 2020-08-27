/**
 * @file		DeviceTools.h
 * @author		Noam Rodrik
 * @description Tools that help manipulate bus devices.
 */
#ifndef __LR35902_DEVICE_TOOLS_H__
#define __LR35902_DEVICE_TOOLS_H__

#include <Core/CPU/Processor.h>
#include <Contrib/GSL/not_null.h>
#include <API/Memory/Memory.h>
#include <API/Definitions.h>
#include <algorithm>

namespace Core
{
class DeviceTools
{
public:
	static void Map(const gsl::not_null<API::data_t*> memory_to_map_ptr, const size_t amount_to_map, const API::address_t address_to_map)
	{
		gsl::not_null<API::IMemoryDevice*> device_at_address = Processor::GetInstance().GetMemory().GetDeviceAtAddress(address_to_map);

		SANITY(amount_to_map <= device_at_address->GetEndAddress() - address_to_map,
			   "Can't map %llu bytes from %04X to %04X!", amount_to_map, address_to_map, device_at_address->GetEndAddress());

		std::copy(memory_to_map_ptr.get(), memory_to_map_ptr.get() + amount_to_map, device_at_address->GetMemoryPointer());
	}
};
} // Core

#endif // __LR35902_DEVICE_TOOLS_H__
