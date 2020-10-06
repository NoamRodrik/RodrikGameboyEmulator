/**
 * @file		MBC1_0x03.h
 * @author		Noam Rodrik
 * @description Implementation of the first mbc.
 */
#ifndef __MBC_ROM_1_RAM_SAVE_H__
#define __MBC_ROM_1_RAM_SAVE_H__

#include <Core/Cartridge/Implementation/MBC1_0x02.h>
#include <API/Cartridge/IMemoryBankController.h>
#include <API/Memory/Memory.h>

namespace Core
{
class MemoryBankController_3 : public API::AMemoryBankController
{
public:
	MemoryBankController_3(IMemoryDeviceAccess& memory_device, std::shared_ptr<API::ILoader> loader) :
		API::AMemoryBankController{memory_device, loader},
		_inner_mbc{memory_device, loader} {}
	virtual ~MemoryBankController_3() = default;

public:
	virtual API::CartridgeHeader::CartridgeType Type() const override { return API::CartridgeHeader::CartridgeType::MBC1_RAM_BATTERY; }
	virtual size_t BankSize() const override;
	virtual void LoadMBC() override;
	virtual void CloseMBC() override;
	virtual bool Read(const API::address_t absolute_address, API::data_t& result) const override;
	virtual bool Write(const API::address_t absolute_address, const API::data_t data) override;
	virtual bool WriteDirectly(const API::address_t absolute_address, const API::data_t data) override;

public:
	static constexpr size_t BANK_SIZE = 125;

private:
	const bool DoesFileExist() const;

private:
	MemoryBankController_2 _inner_mbc;
	std::string            _filename;
};
} // Core

#endif // __MBC_ROM_1_RAM_SAVE_H__