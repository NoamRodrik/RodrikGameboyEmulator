/**
 * @file		MBC_0x01.h
 * @author		Noam Rodrik
 * @description Implementation of the first mbc.
 */
#ifndef __MBC_ROM_1_H__
#define __MBC_ROM_1_H__

#include <API/Cartridge/IMemoryBankController.h>
#include <API/Memory/Memory.h>

namespace Core
{
class MemoryBankController_1 : public API::AMemoryBankController
{
public:
	using API::AMemoryBankController::AMemoryBankController;
	virtual ~MemoryBankController_1() = default;

public:
	virtual API::CartridgeHeader::CartridgeType Type() const override { return API::CartridgeHeader::CartridgeType::MBC1; }
	virtual size_t BankSize() const override;
	virtual void LoadMBC() override;
	virtual bool Read(const API::address_t absolute_address, API::data_t& result) const override;
	virtual bool Write(const API::address_t absolute_address, const API::data_t data) override;

public:
	static constexpr size_t BANK_SIZE = 125;
	static constexpr size_t MBC_SIZE = Tools::MebibytesToBytes(2);

private:
	API::Memory<MBC_SIZE> m_memory;
};
}

#endif // __MBC_ROM_1_H__