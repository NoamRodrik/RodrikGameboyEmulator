/**
 * @file		IMemoryDevice.h
 * @author		Noam Rodrik
 * @description Defines an interfaces for memory device objects.
 */
#ifndef __IMEMORY_DEVICE_H__
#define __IMEMORY_DEVICE_H__

#include <API/Memory/Device/IMemoryDeviceAccess.h>
#include <API/Memory/Device/IMemoryDevice.h>
#include <Tools/Tools.h>
#include <stdint.h>

namespace Core
{
class DeviceTools;
}

namespace API
{
class IMemoryDevice : public IMemoryDeviceAccess
{
public:
	constexpr IMemoryDevice(const API::address_t start_address, const API::address_t end_address, IMemoryDeviceAccess& memory_accessor) :
		_start_address{start_address},
		_end_address{end_address},
		_memory_accessor{memory_accessor}
	{
	}

	virtual ~IMemoryDevice() = default;

	constexpr API::address_t GetStartAddress() const { return this->_start_address; }
	constexpr API::address_t GetEndAddress() const { return this->_end_address; }

protected:
	virtual uint8_t* GetMemoryPointer() = 0;

	IMemoryDeviceAccess& _memory_accessor;

private:
	const API::address_t _start_address;
	const API::address_t _end_address;

private:
	friend class Core::DeviceTools;
};
} // API

#endif // __IMEMORY_DEVICE_H__