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
#include <memory>

namespace API
{
class [[nodiscard]] IMemoryBankController : public IMemoryDeviceAccess
{
public:
	virtual ~IMemoryBankController() = default;

public:
	virtual CartridgeHeader::CartridgeType Type() const = 0;
	virtual size_t BankSize() const = 0;
	virtual void LoadMBC() = 0;
	virtual void CloseMBC() = 0;
};

class AMemoryBankController : public IMemoryBankController
{
public:
	AMemoryBankController(IMemoryDeviceAccess& memory_device, std::shared_ptr<ILoader> loader) :
		_memory_device{memory_device},
		_loader{loader} {}
	virtual ~AMemoryBankController() = default;

public:
	/* By default, doesn't do anything */
	virtual void CloseMBC() override {}

protected:
	IMemoryDeviceAccess& _memory_device;
	std::shared_ptr<ILoader> _loader;
};
} // API

#endif // __API_IMEMORY_BANK_CONTROLLER_H__