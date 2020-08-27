/**
 * @file		ROM_0x00.h
 * @author		Noam Rodrik
 * @description Implementation of the most basic MBC.
 */
#ifndef __MBC_ROM_H__
#define __MBC_ROM_H__

#include <API/Cartridge/IMemoryBankController.h>

namespace Core
{
class MemoryBankController_ROM : public API::AMemoryBankController
{
public:
	using API::AMemoryBankController::AMemoryBankController;
	virtual ~MemoryBankController_ROM() = default;

public:
	virtual API::CartridgeHeader::CartridgeType Type() const override { return API::CartridgeHeader::CartridgeType::ROM; }
	virtual size_t BankSize() const override;
	virtual void LoadMBC() override;
	virtual bool Read(const API::address_t absolute_address, API::data_t& result) const override;
	virtual bool Write(const API::address_t absolute_address, const API::data_t data) override;

public:
	static constexpr size_t BANK_SIZE = 2;
};
}

#endif // __MBC_ROM_H__