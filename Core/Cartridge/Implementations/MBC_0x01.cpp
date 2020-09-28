/**
 * @file		MBC_0x01.cpp
 * @author		Noam Rodrik
 * @description Main logic of MBC1
 * @note        According to: https://retrocomputing.stackexchange.com/questions/11732/how-does-the-gameboys-memory-bank-switching-work
 */
#include "MBC_0x01.h"

#include <Core/Bus/Devices/CartridgeRAM.h>
#include <Core/Bus/DeviceTools.h>
#include <Core/CPU/Processor.h>
#include <Tools/Tools.h>

using namespace API;

namespace Core
{
void MemoryBankController_1::LoadMBC()
{
	this->_loader->Load(this->_rom_memory.GetMemoryPointer(), MBC_SIZE);

	// $0000-$3FFF Always contains the first 16KB of the cartridge, the first memory bank. It is unable to be switched or modified.
	// $4000-7FFFF will contain the second 16KB of the cartridge.
	DeviceTools::Map(this->_rom_memory.GetMemoryPointer(), Tools::BytesInROMBanks(2), CartridgeRAM::START_ADDRESS);
	this->_selected_rom_bank = 1;
} 

size_t MemoryBankController_1::BankSize() const
{
	return MemoryBankController_1::BANK_SIZE;
}

bool MemoryBankController_1::Read(const API::address_t absolute_address, API::data_t& result) const
{
	// We don't need to intercept reads.
	return false;
}

bool MemoryBankController_1::Write(const API::address_t absolute_address, const API::data_t data)
{
	if (absolute_address >= MemoryBankController_1::ROM_LOWER_BANK_NUMBER_START &&
	    absolute_address <= MemoryBankController_1::ROM_LOWER_BANK_NUMBER_END)
	{
		return this->RomLowerBankNumberAction(data);
	}
	else if (absolute_address >= MemoryBankController_1::ROM_UPPER_BANK_NUMBER_START &&
		     absolute_address <= MemoryBankController_1::ROM_UPPER_BANK_NUMBER_END)
	{
		return this->RomUpperBankNumberAction(data);
	}
	else if (absolute_address >= MemoryBankController_1::RAM_ROM_MODE_SELECT_START &&
		     absolute_address <= MemoryBankController_1::RAM_ROM_MODE_SELECT_END)
	{
		// We need to check what mode is selected, then we can intercept the request.
		this->_mode = static_cast<Mode>(data & 0x01);
		
		if (this->_mode == Mode::RAM_MODE)
		{
			this->RomUpperBankNumberAction((this->_selected_rom_bank >> 5) & 0x03);
		}
		else
		{
			// MAP BANK 00 if on ROM mode.
			DeviceTools::Map(this->_rom_memory.GetMemoryPointer(), Tools::BytesInROMBanks(1), CartridgeRAM::START_ADDRESS);
		}

		return true;
	}

	return false;
}

bool MemoryBankController_1::RomUpperBankNumberAction(const data_t data)
{
	// Select the upper 2 bits of the bank number.
	// Selecting a new ROM bank.
	if (this->_mode == Mode::ROM_MODE)
	{
		this->_selected_rom_bank = ((data & 0x03) << 5) | (this->_selected_rom_bank & 0x1F);
		this->LoadSelectedROMBank();
	}
	else
	{
		// Map first the correct bank 0.
		DeviceTools::Map(this->_rom_memory.GetMemoryPointer(), Tools::BytesInROMBanks(1), CartridgeRAM::START_ADDRESS);

		// https://hacktix.github.io/GBEDG/mbcs/mbc1/
		// ... If the ROM is 1MB in size (64 banks) the Zero Bank Number is determined by the lower bit of the 2-bit RAM bank number ...
		// ... If the ROM is 2MB in size (128 banks) the Zero Bank Number is determined by the entire 2-bit RAM bank number ...
		CartridgeHeader header{this->_memory_device};
		if (static_cast<size_t>(header.ROMSize()) >= static_cast<size_t>(CartridgeHeader::ROMSizeValue::_1000_KB))
		{
			const data_t SELECTED_ROM_BANK = static_cast<size_t>(header.ROMSize()) < static_cast<size_t>(CartridgeHeader::ROMSizeValue::_2000_KB) ?
				(data & 0b1) << 5:
				(data & 0b11) << 5;
			const size_t NEW_BANK_OFFSET = Tools::BytesInROMBanks(SELECTED_ROM_BANK);

			SANITY(NEW_BANK_OFFSET + API::MEMORY_ROM_BANK_SIZE < this->_rom_memory.MEMORY_SIZE, "Wrong offset: %llu", NEW_BANK_OFFSET);
			DeviceTools::Map(this->_rom_memory.GetMemoryPointer() + NEW_BANK_OFFSET, Tools::BytesInROMBanks(1), CartridgeRAM::START_ADDRESS);
		}

	}

	return true;
}

bool MemoryBankController_1::RomLowerBankNumberAction(const data_t data)
{
	if (this->_mode == Mode::ROM_MODE)
	{
		// Select the lower 5 bits of the bank number.
		const data_t LOWER_BITS_BANK_NUMBER = data & static_cast<data_t>(0x1F);
		this->_selected_rom_bank = (this->_selected_rom_bank & 0x60) | (LOWER_BITS_BANK_NUMBER == 0x0 ? 0x1 : LOWER_BITS_BANK_NUMBER);
		this->LoadSelectedROMBank();
	}

	return true;
}

void MemoryBankController_1::LoadSelectedROMBank()
{
	DeviceTools::Clear(ADDITIONAL_ROM_BANKS_OFFSET, API::MEMORY_ROM_BANK_SIZE);

	if (this->_selected_rom_bank != 0x00)
	{
		// Mapping the new bank into the cartridge.
		const size_t NEW_BANK_OFFSET{Tools::BytesInROMBanks(this->_selected_rom_bank)};
		SANITY(NEW_BANK_OFFSET + API::MEMORY_ROM_BANK_SIZE < this->_rom_memory.MEMORY_SIZE, "Wrong offset: %llu", NEW_BANK_OFFSET);
		DeviceTools::Map(this->_rom_memory.GetMemoryPointer() + NEW_BANK_OFFSET, API::MEMORY_ROM_BANK_SIZE, ADDITIONAL_ROM_BANKS_OFFSET);
	}
}
}