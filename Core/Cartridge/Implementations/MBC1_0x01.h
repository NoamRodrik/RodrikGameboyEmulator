/**
 * @file		MBC1_0x01.h
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
	virtual bool WriteDirectly(const API::address_t absolute_address, const API::data_t data) override;

private:
	enum class Mode
	{
		ROM_MODE = 0x00,
		RAM_MODE = 0x01
	};

public:
	static constexpr size_t BANK_SIZE = 125;
	static constexpr size_t MBC_SIZE = Tools::MebibytesToBytes(16);

private:
	bool RomLowerBankNumberAction(const API::data_t data);
	bool RomUpperBankNumberAction(const API::data_t data);

private:
	static constexpr API::address_t ADDITIONAL_ROM_BANKS_OFFSET = 0x4000;
	static constexpr API::address_t ROM_LOWER_BANK_NUMBER_START = 0x2000;
	static constexpr API::address_t ROM_LOWER_BANK_NUMBER_END = 0x3FFF;
	static constexpr API::address_t ROM_UPPER_BANK_NUMBER_START = 0x4000;
	static constexpr API::address_t ROM_UPPER_BANK_NUMBER_END = 0x5FFF;
	static constexpr API::address_t RAM_ROM_MODE_SELECT_START = 0x6000;
	static constexpr API::address_t RAM_ROM_MODE_SELECT_END = 0x7FFF;

private:
	API::Memory<MBC_SIZE> _rom_memory;
	Mode				  _mode{Mode::ROM_MODE};
	API::data_t			  _selected_rom_bank{0x01};
	API::data_t           _selected_zero_bank{0x00};

private:
	friend class MemoryBankController_2;
	friend class MemoryBankController_3;
};
} // Core

#endif // __MBC_ROM_1_H__