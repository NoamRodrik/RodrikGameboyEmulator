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
	enum class Mode
	{
		ROM_MODE = 0x00,
		RAM_MODE = 0x01
	};

private:
	bool RamRomBankNumberAction(const API::data_t data);
	bool RomBankNumberAction(const API::data_t data);
	void LoadSelectedROMBank();
	void SaveSelectedRAMBank();
	void LoadSelectedRAMBank();

private:
	static constexpr API::address_t ADDITIONAL_ROM_BANKS_OFFSET = 0x4000;
	static constexpr API::address_t ADDITIONAL_RAM_BANKS_OFFSET = 0xA000;
	static constexpr API::address_t RAM_ENABLE_START = 0x0000;
	static constexpr API::address_t RAM_ENABLE_END = 0x1FFF;
	static constexpr API::address_t ROM_BANK_NUMBER_START = 0x2000;
	static constexpr API::address_t ROM_BANK_NUMBER_END = 0x3FFF;
	static constexpr API::address_t RAM_ROM_BANK_NUMBER_START = 0x4000;
	static constexpr API::address_t RAM_ROM_BANK_NUMBER_END = 0x5FFF;
	static constexpr API::address_t RAM_ROM_MODE_SELECT_START = 0x6000;
	static constexpr API::address_t RAM_ROM_MODE_SELECT_END = 0x7FFF;

private:
	API::Memory<MBC_SIZE>				   m_rom_memory;
	API::Memory<Tools::BytesInRAMBanks(4)> m_ram_memory;
	API::data_t							   m_selected_rom_bank{0};
	API::data_t							   m_selected_ram_bank{0};
	Mode								   m_mode{Mode::ROM_MODE};
	bool								   m_ram_enable{false};
};
}

#endif // __MBC_ROM_1_H__