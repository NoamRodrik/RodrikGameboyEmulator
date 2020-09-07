/**
 * @file		MBC1_0x02.cpp
 * @author		Noam Rodrik
 * @description Main logic of MBC1 with RAM
 * @note        According to: https://retrocomputing.stackexchange.com/questions/11732/how-does-the-gameboys-memory-bank-switching-work
 */
#include "MBC1_0x02.h"

#include <Core/Bus/Devices/CartridgeRAM.h>
#include <Core/Bus/DeviceTools.h>
#include <Core/CPU/Processor.h>
#include <Tools/Tools.h>

using namespace API;

namespace Core
{
void MemoryBankController_2::LoadMBC()
{
	this->m_inner_mbc.LoadMBC();
} 

size_t MemoryBankController_2::BankSize() const
{
	return MemoryBankController_2::BANK_SIZE;
}

bool MemoryBankController_2::Read(const API::address_t absolute_address, API::data_t& result) const
{
	// We don't need to intercept reads.
	return false;
}

bool MemoryBankController_2::Write(const API::address_t absolute_address, const API::data_t data)
{
	if (absolute_address >= MemoryBankController_2::RAM_ENABLE_START &&
		absolute_address <= MemoryBankController_2::RAM_ENABLE_END)
	{
		// To enable RAM, write a value with the lower 4 bits being $0A somewhere in this address space.
		// To disable RAM, any number except $0A can be written.
		// It does not matter where it is written, just as long as it within the address range.
		this->m_ram_enable = ((data & 0x0F) == 0x0A);
		return true;
	}
	else if (absolute_address >= MemoryBankController_2::ROM_BANK_NUMBER_START &&
		     absolute_address <= MemoryBankController_2::ROM_BANK_NUMBER_END)
	{
		if (this->m_mode == Mode::RAM_MODE)
		{
			this->m_inner_mbc.m_selected_rom_bank &= 0x1F;
		}

		return this->m_inner_mbc.RomLowerBankNumberAction(data);
	}
	else if (absolute_address >= MemoryBankController_2::RAM_ROM_BANK_NUMBER_START &&
		     absolute_address <= MemoryBankController_2::RAM_ROM_BANK_NUMBER_END)
	{
		return this->RamRomBankNumberAction(data);
	}
	else if (absolute_address >= MemoryBankController_2::RAM_ROM_MODE_SELECT_START &&
		     absolute_address <= MemoryBankController_2::RAM_ROM_MODE_SELECT_END)
	{
		// We need to check what mode is selected, then we can intercept the request.
		this->m_mode = static_cast<Mode>(data & 0x01);

		if (this->m_mode == Mode::ROM_MODE)
		{
			// Only RAM Bank 00h can be used during ROM mode.
			this->SaveSelectedRAMBank();
			this->m_selected_ram_bank = 0;
			this->LoadSelectedRAMBank();
		}

		return true;
	}

	return false;
}

bool MemoryBankController_2::RamRomBankNumberAction(const data_t data)
{
	// If in ROM mode (no RAM bank switching), it will specify the upper two bits of the ROM bank number.
	if (this->m_mode == Mode::ROM_MODE)
	{
		this->m_inner_mbc.RomUpperBankNumberAction(data);
	}
	else if (this->m_mode == Mode::RAM_MODE && this->m_ram_enable)
	{	
		this->SaveSelectedRAMBank();
		this->m_selected_ram_bank = (data & 0x03);
		this->LoadSelectedRAMBank();
	}
	else
	{
		STOP_RUNNING("Not supposed to be here.");
	}

	return true;
}

void MemoryBankController_2::SaveSelectedRAMBank()
{
	const size_t OLDER_BANK_OFFSET{Tools::BytesInRAMBanks(this->m_selected_ram_bank)};
	SANITY(OLDER_BANK_OFFSET + API::MEMORY_RAM_BANK_SIZE < this->m_ram_memory.MEMORY_SIZE, "Wrong offset");
	
	// Unmap the older bank.
	DeviceTools::Unmap(this->m_ram_memory.GetMemoryPointer() + OLDER_BANK_OFFSET, API::MEMORY_RAM_BANK_SIZE, ADDITIONAL_RAM_BANKS_OFFSET);
}

void MemoryBankController_2::LoadSelectedRAMBank()
{
	DeviceTools::Clear(ADDITIONAL_RAM_BANKS_OFFSET, API::MEMORY_RAM_BANK_SIZE);

	// Mapping the new bank into the cartridge.
	const size_t NEW_BANK_OFFSET{Tools::BytesInRAMBanks(this->m_selected_ram_bank)};
	SANITY(NEW_BANK_OFFSET + API::MEMORY_RAM_BANK_SIZE < this->m_ram_memory.MEMORY_SIZE, "Wrong offset");
	DeviceTools::Map(this->m_ram_memory.GetMemoryPointer() + NEW_BANK_OFFSET, API::MEMORY_RAM_BANK_SIZE, ADDITIONAL_RAM_BANKS_OFFSET);
}
}