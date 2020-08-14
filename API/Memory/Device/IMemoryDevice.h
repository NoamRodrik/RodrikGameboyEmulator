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

namespace API
{
class IMemoryDevice : public IMemoryDeviceAccess
{
public:
	constexpr IMemoryDevice(const uint32_t start_address, const uint32_t end_address, IMemoryDeviceAccess& memory_accessor) :
		m_start_address{start_address},
		m_end_address{end_address},
		m_memory_accessor{memory_accessor}
	{
	}

	virtual ~IMemoryDevice() = default;

	constexpr uint32_t GetStartAddress() const { return this->m_start_address; }
	constexpr uint32_t GetEndAddress() const { return this->m_end_address; }

protected:
	virtual uint8_t* GetMemoryPointer() = 0;

	IMemoryDeviceAccess& m_memory_accessor;

private:
	const uint32_t m_start_address;
	const uint32_t m_end_address;
};
} // API

#endif // __IMEMORY_DEVICE_H__