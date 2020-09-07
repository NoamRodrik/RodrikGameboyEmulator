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
	static void Map(const gsl::not_null<API::data_t*> memory_to_map_ptr, const std::size_t amount_to_map, const API::address_t address_to_map)
	{
		gsl::not_null<API::IMemoryDevice*> device_at_address = Processor::GetInstance().GetMemory().GetDeviceAtAddress(address_to_map);

		SANITY(amount_to_map <= static_cast<std::size_t>(device_at_address->GetEndAddress()) - static_cast<std::size_t>(address_to_map) + 1,
			   "Can't map %llu bytes from %04X to %04X!", amount_to_map, address_to_map, device_at_address->GetEndAddress());

		std::copy(memory_to_map_ptr.get(), memory_to_map_ptr.get() + amount_to_map, device_at_address->GetMemoryPointer() + address_to_map);
	}

	static void Unmap(gsl::not_null<API::data_t*> memory_to_unmap_ptr, const API::address_t amount_to_unmap, const API::address_t address_to_unmap)
	{
		gsl::not_null<API::IMemoryDevice*> device_at_address = Processor::GetInstance().GetMemory().GetDeviceAtAddress(address_to_unmap);

		SANITY(amount_to_unmap <= device_at_address->GetEndAddress() - address_to_unmap + 1,
			   "Can't unmap %hu bytes from %04X to %04X!", amount_to_unmap, address_to_unmap, device_at_address->GetEndAddress());

		std::copy(device_at_address->GetMemoryPointer() + address_to_unmap, device_at_address->GetMemoryPointer() + address_to_unmap + amount_to_unmap, memory_to_unmap_ptr.get());
	}

	static void Clear(const API::address_t address, const API::address_t amount_to_clear)
	{
		gsl::not_null<API::IMemoryDevice*> device_at_address = Processor::GetInstance().GetMemory().GetDeviceAtAddress(address);

		SANITY(amount_to_clear <= device_at_address->GetEndAddress() - address + 1,
			"Can't unmap %hu bytes from %04X to %04X!", amount_to_clear, address, device_at_address->GetEndAddress());

		std::fill(device_at_address->GetMemoryPointer() + address, device_at_address->GetMemoryPointer() + address + amount_to_clear, 0);
	}
};
} // Core

#endif // __LR35902_DEVICE_TOOLS_H__
