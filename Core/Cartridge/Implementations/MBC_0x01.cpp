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

	return false;
}

bool MemoryBankController_1::RomUpperBankNumberAction(const data_t data)
{
	// Select the upper 2 bits of the bank number.
	// Selecting a new ROM bank.
	this->_selected_rom_bank = ((data & 0x03) << 5) | (this->_selected_rom_bank & 0x1F);
	this->LoadSelectedROMBank();

	return true;
}

bool MemoryBankController_1::RomLowerBankNumberAction(const data_t data)
{
	CartridgeHeader cartridge_header{this->_memory_device};

	// Select the lower 5 bits of the bank number.
	const data_t LOWER_BITS_BANK_NUMBER = data & static_cast<data_t>(0x1F);
	this->_selected_rom_bank = (this->_selected_rom_bank & 0x60) | (LOWER_BITS_BANK_NUMBER == 0x0 ? 0x1 : LOWER_BITS_BANK_NUMBER);
	this->_selected_rom_bank &= (static_cast<std::size_t>(cartridge_header.ROMSize()) / 16) - 1;
	this->LoadSelectedROMBank();

	return true;
}

void MemoryBankController_1::LoadSelectedROMBank()
{
	DeviceTools::Clear(ADDITIONAL_ROM_BANKS_OFFSET, API::MEMORY_ROM_BANK_SIZE);

	if (this->_selected_rom_bank != 0x00)
	{
		// Mapping the new bank into the cartridge.
		const size_t NEW_BANK_OFFSET{Tools::BytesInROMBanks(this->_selected_rom_bank)};
		SANITY(NEW_BANK_OFFSET + API::MEMORY_ROM_BANK_SIZE < this->_rom_memory.MEMORY_SIZE, "Wrong offset");
		DeviceTools::Map(this->_rom_memory.GetMemoryPointer() + NEW_BANK_OFFSET, API::MEMORY_ROM_BANK_SIZE, ADDITIONAL_ROM_BANKS_OFFSET);
	}
}
}