/**
 * @file		IMemoryBankController.h
 * @author		Noam Rodrik
 * @description API of a generic memory bank controller.
 */
#ifndef __API_IMEMORY_BANK_CONTROLLER_H__
#define __API_IMEMORY_BANK_CONTROLLER_H__

#include <API/Memory/Device/IMemoryDeviceAccess.h>
#include <API/Cartridge/ILoader.h>
#include <API/Cartridge/Header.h>
#include <cstdint>

namespace API
{
class IMemoryBankController
{
public:
	virtual ~IMemoryBankController() = default;

public:
	virtual CartridgeHeader::CartridgeType Type() const = 0;
	virtual void LoadROMBankZero() = 0;
};

class AMemoryBankController : public IMemoryBankController
{
public:
	AMemoryBankController(IMemoryDeviceAccess& memory_device, ILoader& loader) :
		m_memory_device{memory_device},
		m_loader{loader} {}
	virtual ~AMemoryBankController() = default;

protected:
	IMemoryDeviceAccess& m_memory_device;
	ILoader& m_loader;
};
}

#endif // __API_IMEMORY_BANK_CONTROLLER_H__