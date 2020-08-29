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
	this->m_loader->Load(this->m_memory.GetMemoryPointer(), MBC_SIZE);

	// $0000-$3FFF Always contains the first 16KB of the cartridge, the first memory bank. It is unable to be switched or modified.
	DeviceTools::Map(this->m_memory.GetMemoryPointer(), API::MEMORY_BANK_SIZE, CartridgeRAM::START_ADDRESS);
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
	if (absolute_address >= MemoryBankController_1::RAM_ENABLE_START &&
		absolute_address <= MemoryBankController_1::RAM_ENABLE_END)
	{
		// To enable RAM, write a value with the lower 4 bits being $0A somewhere in this address space.
		// To disable RAM, any number except $0A can be written.
		// It does not matter where it is written, just as long as it within the address range.
		this->m_ram_enable = ((data & 0x0A) == 0x0A);
		return true;
	}
	else if (absolute_address >= MemoryBankController_1::ROM_BANK_NUMBER_START &&
		     absolute_address <= MemoryBankController_1::ROM_BANK_NUMBER_END)
	{
		return this->RomBankNumberAction(data);
	}
	else if (absolute_address >= MemoryBankController_1::RAM_ROM_MODE_SELECT_START &&
		     absolute_address <= MemoryBankController_1::RAM_ROM_MODE_SELECT_END)
	{
		SANITY(data >= 0x00 && data <= 0x01, "Got invalid values for data");

		// We need to check what mode is selected, then we can intercept the request.
		this->m_mode = static_cast<Mode>(data);
		return true;
	}
	else if (absolute_address >= MemoryBankController_1::RAM_ROM_BANK_NUMBER_START &&
			 absolute_address <= MemoryBankController_1::RAM_ROM_BANK_NUMBER_END)
	{
		return this->RamRomBankNumberAction(data);
	}

	return false;
}

bool MemoryBankController_1::RamRomBankNumberAction(const data_t data)
{
	// If in ROM mode (no RAM bank switching), it will specify the upper two bits of the ROM bank number.
	// In this mode, only RAM bank $00 may be used.
	if (this->m_mode == Mode::ROM_MODE)
	{
		// Select the upper 2 bits of the bank number.
		const data_t UPPER_BITS_BANK_NUMBER = (data & 0x03) << 5;

		// Saving and selecting a new bank.
		this->SaveSelectedROMBank();
		this->m_selected_rom_bank = this->m_selected_rom_bank | UPPER_BITS_BANK_NUMBER;
		this->LoadSelectedROMBank();
	}
	else if (this->m_mode == Mode::RAM_MODE)
	{
		Message("TODO RAM mode");
	}
	else
	{
		STOP_RUNNING("Not supposed to be here.");
	}

	return true;
}

bool MemoryBankController_1::RomBankNumberAction(const data_t data)
{
	// Select the lower 5 bits of the bank number.
	const data_t LOWER_BITS_BANK_NUMBER = data & static_cast<data_t>(0x1F);

	this->SaveSelectedROMBank();
	this->m_selected_rom_bank = this->m_selected_rom_bank | LOWER_BITS_BANK_NUMBER;
	size_t new_bank_offset{Tools::BytesInBanks(this->m_selected_rom_bank)};

	// Apply bug.
	switch (LOWER_BITS_BANK_NUMBER)
	{
		// When these values are written,
		// instead of addressing the correct ROM banks
		// they will address banks n+1 instead.
		case (0x00):
		case (0x20):
		case (0x40):
		case (0x60):
		{
			this->m_selected_rom_bank += 1;

			[[fallthrough]];
		}
		default:
		{
			break;
		}
	}

	this->LoadSelectedROMBank();

	// Undo bug.
	switch (LOWER_BITS_BANK_NUMBER)
	{
		case (0x00):
		case (0x20):
		case (0x40):
		case (0x60):
		{
			this->m_selected_rom_bank -= 1;

			[[fallthrough]];
		}
		default:
		{
			break;
		}
	}

	return true;
}

void MemoryBankController_1::SaveSelectedROMBank()
{
	// If we loaded an earlier bank.
	if (this->m_selected_rom_bank != 0)
	{
		const size_t OLDER_BANK_OFFSET{Tools::BytesInBanks(this->m_selected_rom_bank)};
		SANITY(OLDER_BANK_OFFSET < this->m_memory.MEMORY_SIZE, "Wrong offset");
	
		// Unmap the older bank.
		DeviceTools::Unmap(this->m_memory.GetMemoryPointer() + OLDER_BANK_OFFSET, API::MEMORY_BANK_SIZE, ADDITIONAL_BANKS_OFFSET);
	}
}

void MemoryBankController_1::LoadSelectedROMBank()
{
	// Mapping the new bank into the cartridge.
	const size_t NEW_BANK_OFFSET{Tools::BytesInBanks(this->m_selected_rom_bank)};
	SANITY(NEW_BANK_OFFSET < this->m_memory.MEMORY_SIZE, "Wrong offset");
	DeviceTools::Map(this->m_memory.GetMemoryPointer() + NEW_BANK_OFFSET, API::MEMORY_BANK_SIZE, ADDITIONAL_BANKS_OFFSET);
}
}