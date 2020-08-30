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
	this->m_loader->Load(this->m_rom_memory.GetMemoryPointer(), MBC_SIZE);

	// $0000-$3FFF Always contains the first 16KB of the cartridge, the first memory bank. It is unable to be switched or modified.
	// $4000-7FFFF will contain the second 16KB of the cartridge.
	DeviceTools::Map(this->m_rom_memory.GetMemoryPointer(), Tools::BytesInROMBanks(2), CartridgeRAM::START_ADDRESS);
	this->m_selected_rom_bank = 1;
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
	else if (absolute_address >= MemoryBankController_1::RAM_ROM_BANK_NUMBER_START &&
		     absolute_address <= MemoryBankController_1::RAM_ROM_BANK_NUMBER_END)
	{
		return this->RamRomBankNumberAction(data);
	}
	else if (absolute_address >= MemoryBankController_1::RAM_ROM_MODE_SELECT_START &&
		     absolute_address <= MemoryBankController_1::RAM_ROM_MODE_SELECT_END)
	{
		SANITY(data >= 0x00 && data <= 0x01, "Got invalid values for data");

		// We need to check what mode is selected, then we can intercept the request.
		this->m_mode = static_cast<Mode>(data);

		switch (this->m_mode)
		{
			case (Mode::ROM_MODE):
			{
				// Only RAM Bank 00h can be used during ROM mode.
				this->SaveSelectedRAMBank();
				this->m_selected_ram_bank = 0;
				this->LoadSelectedRAMBank();

				break;
			}

			case (Mode::RAM_MODE):
			{
				// Only ROM Banks 00-1Fh can be used during RAM mode.
				this->m_selected_rom_bank &= 0x1F;
				this->LoadSelectedROMBank();

				break;
			}
			
			default:
			{
				STOP_RUNNING("Not supposed to be here");
			}
		}

		return true;
	}

	return false;
}

bool MemoryBankController_1::RamRomBankNumberAction(const data_t data)
{
	// If in ROM mode (no RAM bank switching), it will specify the upper two bits of the ROM bank number.
	if (this->m_mode == Mode::ROM_MODE)
	{
		// Select the upper 2 bits of the bank number.
		// Selecting a new ROM bank.
		this->m_selected_rom_bank = ((data & 0x03) << 5) | (this->m_selected_rom_bank & 0x1F);
		this->LoadSelectedROMBank();
	}
	else if (this->m_mode == Mode::RAM_MODE && this->m_ram_enable)
	{	
		this->SaveSelectedRAMBank();
		this->m_selected_ram_bank = data;
		this->LoadSelectedRAMBank();
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
	this->m_selected_rom_bank = (this->m_selected_rom_bank & 0x60) | (LOWER_BITS_BANK_NUMBER == 0x0 ? 0x1 : LOWER_BITS_BANK_NUMBER);
	this->LoadSelectedROMBank();

	return true;
}

void MemoryBankController_1::LoadSelectedROMBank()
{
	DeviceTools::Clear(ADDITIONAL_ROM_BANKS_OFFSET, API::MEMORY_ROM_BANK_SIZE);

	// Mapping the new bank into the cartridge.
	const size_t NEW_BANK_OFFSET{Tools::BytesInROMBanks(this->m_selected_rom_bank)};
	SANITY(NEW_BANK_OFFSET + API::MEMORY_ROM_BANK_SIZE < this->m_rom_memory.MEMORY_SIZE, "Wrong offset");
	DeviceTools::Map(this->m_rom_memory.GetMemoryPointer() + NEW_BANK_OFFSET, API::MEMORY_ROM_BANK_SIZE, ADDITIONAL_ROM_BANKS_OFFSET);
}

void MemoryBankController_1::SaveSelectedRAMBank()
{
	const size_t OLDER_BANK_OFFSET{Tools::BytesInRAMBanks(this->m_selected_ram_bank)};
	SANITY(OLDER_BANK_OFFSET + API::MEMORY_RAM_BANK_SIZE < this->m_ram_memory.MEMORY_SIZE, "Wrong offset");
	
	// Unmap the older bank.
	DeviceTools::Unmap(this->m_ram_memory.GetMemoryPointer() + OLDER_BANK_OFFSET, API::MEMORY_RAM_BANK_SIZE, ADDITIONAL_RAM_BANKS_OFFSET);
}

void MemoryBankController_1::LoadSelectedRAMBank()
{
	DeviceTools::Clear(ADDITIONAL_RAM_BANKS_OFFSET, API::MEMORY_RAM_BANK_SIZE);

	// Mapping the new bank into the cartridge.
	const size_t NEW_BANK_OFFSET{Tools::BytesInRAMBanks(this->m_selected_ram_bank)};
	SANITY(NEW_BANK_OFFSET + API::MEMORY_RAM_BANK_SIZE < this->m_ram_memory.MEMORY_SIZE, "Wrong offset");
	DeviceTools::Map(this->m_ram_memory.GetMemoryPointer() + NEW_BANK_OFFSET, API::MEMORY_RAM_BANK_SIZE, ADDITIONAL_RAM_BANKS_OFFSET);
}
}