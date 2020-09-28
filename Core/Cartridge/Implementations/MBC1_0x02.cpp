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
	this->_inner_mbc.LoadMBC();
} 

size_t MemoryBankController_2::BankSize() const
{
	return MemoryBankController_2::BANK_SIZE;
}

bool MemoryBankController_2::Read(const API::address_t absolute_address, API::data_t& result) const
{
	return this->_inner_mbc.Read(absolute_address, result);
}

bool MemoryBankController_2::Write(const API::address_t absolute_address, const API::data_t data)
{
	if (absolute_address >= MemoryBankController_2::RAM_ENABLE_START &&
		absolute_address <= MemoryBankController_2::RAM_ENABLE_END)
	{
		// To enable RAM, write a value with the lower 4 bits being $0A somewhere in this address space.
		// To disable RAM, any number except $0A can be written.
		// It does not matter where it is written, just as long as it within the address range.
		this->_ram_enable = ((data & 0x0F) == 0x0A);
		return true;
	}
	else if (absolute_address >= MemoryBankController_2::ROM_BANK_NUMBER_START &&
		     absolute_address <= MemoryBankController_2::ROM_BANK_NUMBER_END)
	{
		RET_FALSE_IF_FAIL(this->_inner_mbc.RomLowerBankNumberAction(data), "Failed setting lower bank number");

		CartridgeHeader header{this->_memory_device};

		API::data_t mask{0x1F};
		switch (header.ROM())
		{
			case (CartridgeHeader::ROMSizeType::_64_KB):
			{
				mask = 0b11;
				break;
			}
			
			case (CartridgeHeader::ROMSizeType::_128_KB):
			{
				mask = 0b111;
				break;
			}

			case (CartridgeHeader::ROMSizeType::_256_KB):
			{
				mask = 0b1111;
				break;
			}
		}

		this->_inner_mbc._selected_rom_bank &= mask;

		return true;
	}
	else if (absolute_address >= MemoryBankController_2::RAM_ROM_BANK_NUMBER_START &&
		     absolute_address <= MemoryBankController_2::RAM_ROM_BANK_NUMBER_END)
	{
		return this->RamRomBankNumberAction(data);
	}
	else if (absolute_address >= MemoryBankController_1::RAM_ROM_MODE_SELECT_START &&
		     absolute_address <= MemoryBankController_1::RAM_ROM_MODE_SELECT_END)
	{
		// TODO call write
		RET_FALSE_IF_FAIL(this->_inner_mbc.Write(absolute_address, data), "Failed writing to inner mbc");

		if (this->_inner_mbc._mode == MemoryBankController_1::Mode::ROM_MODE)
		{
			// Only RAM Bank 00h can be used during ROM mode.
			this->SaveSelectedRAMBank();
			this->_selected_ram_bank = 0;
			this->LoadSelectedRAMBank();
		}

		return true;
	}
	else if (absolute_address >= MemoryBankController_1::ROM_LOWER_BANK_NUMBER_START &&
		absolute_address <= MemoryBankController_1::ROM_LOWER_BANK_NUMBER_END)
	{
		return this->_inner_mbc.RomLowerBankNumberAction(data);
	}
	else if (absolute_address >= MemoryBankController_1::ROM_UPPER_BANK_NUMBER_START &&
		absolute_address <= MemoryBankController_1::ROM_UPPER_BANK_NUMBER_END)
	{
		return this->_inner_mbc.RomUpperBankNumberAction(data);
	}

	return false;
}

bool MemoryBankController_2::RamRomBankNumberAction(const data_t data)
{
	// If in ROM mode (no RAM bank switching), it will specify the upper two bits of the ROM bank number.
	if (this->_inner_mbc._mode == MemoryBankController_1::Mode::ROM_MODE)
	{
		this->_inner_mbc.RomUpperBankNumberAction(data);
	}
	else if (this->_inner_mbc._mode == MemoryBankController_1::Mode::RAM_MODE && this->_ram_enable)
	{	
		this->SaveSelectedRAMBank();
		this->_selected_ram_bank = (data & 0x03);
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
	const size_t OLDER_BANK_OFFSET{Tools::BytesInRAMBanks(this->_selected_ram_bank)};
	SANITY(OLDER_BANK_OFFSET + API::MEMORY_RAM_BANK_SIZE < this->_ram_memory.MEMORY_SIZE, "Wrong offset");
	
	// Unmap the older bank.
	DeviceTools::Unmap(this->_ram_memory.GetMemoryPointer() + OLDER_BANK_OFFSET, API::MEMORY_RAM_BANK_SIZE, ADDITIONAL_RAM_BANKS_OFFSET);
}

void MemoryBankController_2::LoadSelectedRAMBank()
{
	DeviceTools::Clear(ADDITIONAL_RAM_BANKS_OFFSET, API::MEMORY_RAM_BANK_SIZE);

	// Mapping the new bank into the cartridge.
	const size_t NEW_BANK_OFFSET{Tools::BytesInRAMBanks(this->_selected_ram_bank)};
	SANITY(NEW_BANK_OFFSET + API::MEMORY_RAM_BANK_SIZE < this->_ram_memory.MEMORY_SIZE, "Wrong offset");
	DeviceTools::Map(this->_ram_memory.GetMemoryPointer() + NEW_BANK_OFFSET, API::MEMORY_RAM_BANK_SIZE, ADDITIONAL_RAM_BANKS_OFFSET);
}
}