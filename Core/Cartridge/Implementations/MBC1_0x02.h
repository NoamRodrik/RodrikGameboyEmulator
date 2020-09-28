/**
 * @file		MBC1_0x02.h
 * @author		Noam Rodrik
 * @description Implementation of the first mbc.
 */
#ifndef __MBC_ROM_1_RAM_H__
#define __MBC_ROM_1_RAM_H__

#include <API/Cartridge/IMemoryBankController.h>
#include <API/Memory/Memory.h>
#include <Core\Cartridge\Implementations\MBC_0x01.h>

namespace Core
{
class MemoryBankController_2 : public API::AMemoryBankController
{
public:
	MemoryBankController_2(IMemoryDeviceAccess& memory_device, std::shared_ptr<API::ILoader> loader) :
		API::AMemoryBankController{memory_device, loader},
		_inner_mbc{memory_device, loader} {}
	virtual ~MemoryBankController_2() = default;

public:
	virtual API::CartridgeHeader::CartridgeType Type() const override { return API::CartridgeHeader::CartridgeType::MBC1_RAM; }
	virtual size_t BankSize() const override;
	virtual void LoadMBC() override;
	virtual bool Read(const API::address_t absolute_address, API::data_t& result) const override;
	virtual bool Write(const API::address_t absolute_address, const API::data_t data) override;

public:
	static constexpr size_t BANK_SIZE = 125;

private:
	bool RamRomBankNumberAction(const API::data_t data);
	void SaveSelectedRAMBank();
	void LoadSelectedRAMBank();

private:
	static constexpr API::address_t ADDITIONAL_RAM_BANKS_OFFSET = 0xA000;
	static constexpr API::address_t RAM_ENABLE_START = 0x0000;
	static constexpr API::address_t RAM_ENABLE_END = 0x1FFF;
	static constexpr API::address_t ROM_BANK_NUMBER_START = 0x2000;
	static constexpr API::address_t ROM_BANK_NUMBER_END = 0x3FFF;
	static constexpr API::address_t RAM_ROM_BANK_NUMBER_START = 0x4000;
	static constexpr API::address_t RAM_ROM_BANK_NUMBER_END = 0x5FFF;

private:
	MemoryBankController_1                 _inner_mbc;
	API::Memory<Tools::BytesInRAMBanks(4)> _ram_memory;
	API::data_t							   _selected_ram_bank{0};
	bool								   _ram_enable{false};
};
}

#endif // __MBC_ROM_1_RAM_H__