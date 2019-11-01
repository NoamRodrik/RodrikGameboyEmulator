/**
 * @file		MemoryDeviceBase.h
 * @author		Noam Rodrik
 * @description Defines basic actions for memory device base objects.
 */
#ifndef __MEMORY_DEVICE_BASE_H__
#define __MEMORY_DEVICE_BASE_H__

#include <Core/API/Memory/Device/IMemoryDeviceAccess.h>
#include <Core/API/Memory/Device/MemoryDeviceBase.h>
#include <Tools/Tools.h>
#include <stdint.h>

namespace Core
{
class DeviceManagerBase;

class MemoryDeviceBase : public IMemoryDeviceAccess
{
public:
	constexpr MemoryDeviceBase(const uint32_t start_address, const uint32_t end_address, DeviceManagerBase& device_manager) :
		m_start_address{start_address},
		m_end_address{end_address},
		m_device_manager{device_manager}
	{
	}

	virtual ~MemoryDeviceBase() = default;

	constexpr uint32_t GetStartAddress() const { return this->m_start_address; }
	constexpr uint32_t GetEndAddress() const { return this->m_end_address; }

protected:
	virtual uint8_t* GetMemoryPointer() = 0;
	DeviceManagerBase& m_device_manager;

private:
	const uint32_t m_start_address;
	const uint32_t m_end_address;

private:
	friend class GameLoader;
};
} // Core

#endif // __MEMORY_DEVICE_BASE_H__