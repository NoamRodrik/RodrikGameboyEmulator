/**
 * @file		MBC1_0x02.cpp
 * @author		Noam Rodrik
 * @description Main logic of MBC1 with RAM
 * @note        According to: https://retrocomputing.stackexchange.com/questions/11732/how-does-the-gameboys-memory-bank-switching-work
 */
#include "MBC1_0x02.h"

#include <Core/Bus/Devices/CartridgeRAM.h>
#include <Core/Bus/Devices/ExternalRAM.h>
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
	if (absolute_address >= CartridgeRAM::START_ADDRESS &&
		absolute_address <= CartridgeRAM::END_ADDRESS)
	{
		return this->_inner_mbc.Read(absolute_address, result);
	}
	else if (absolute_address >= ExternalRAM::START_ADDRESS &&
			 absolute_address <= ExternalRAM::END_ADDRESS)
	{
		// If RAM isn't enabled, 0xFF is read.
		if (!this->_ram_enable)
		{
			result = 0xFF;
			return true;
		}

		if (this->_inner_mbc._mode == MemoryBankController_1::Mode::ROM_MODE)
		{
			SANITY(absolute_address - ExternalRAM::START_ADDRESS < this->_ram_memory.MEMORY_SIZE,
				"Accessed invalid RAM memory");
			result = this->_ram_memory[absolute_address - ExternalRAM::START_ADDRESS];
			return true;
		}
		else
		{
			const size_t BASE_BANK_OFFSET{Tools::BytesInRAMBanks(this->_selected_ram_bank)};
			SANITY(BASE_BANK_OFFSET + (absolute_address - ExternalRAM::START_ADDRESS) < this->_ram_memory.MEMORY_SIZE, 
				   "Accessed invalid RAM memory");
			result = this->_ram_memory[BASE_BANK_OFFSET + (absolute_address - ExternalRAM::START_ADDRESS)];
			return true;
		}
	}

	return false;
}

bool MemoryBankController_2::WriteDirectly(const API::address_t absolute_address, const API::data_t data)
{
	// For MBC, write directly is just write....
	return this->Write(absolute_address, data);
}

bool MemoryBankController_2::Write(const API::address_t absolute_address, const API::data_t data)
{
	// If the inner didnt' handle, let's try.
	if (!this->_inner_mbc.Write(absolute_address, data))
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
	}
	else if (absolute_address >= MemoryBankController_2::RAM_ROM_BANK_NUMBER_START &&
			 absolute_address <= MemoryBankController_2::RAM_ROM_BANK_NUMBER_END)
	{
		SANITY(this->RamRomBankNumberAction(data), "Failed setting bank");
		return true;
	}
	else if (absolute_address <= ExternalRAM::START_ADDRESS &&
		     absolute_address >= ExternalRAM::END_ADDRESS)
	{
		// If RAM isn't enabled, write is ignored.
		if (!this->_ram_enable)
		{
			return true;
		}

		if (this->_inner_mbc._mode == MemoryBankController_1::Mode::ROM_MODE)
		{
			this->_ram_memory[absolute_address - ExternalRAM::START_ADDRESS];
			return true;
		}
		else
		{
			const size_t BASE_BANK_OFFSET{Tools::BytesInRAMBanks(this->_selected_ram_bank)};
			SANITY(BASE_BANK_OFFSET + (absolute_address - ExternalRAM::START_ADDRESS) < this->_ram_memory.MEMORY_SIZE, 
				   "Accessed invalid RAM memory");
			this->_ram_memory[BASE_BANK_OFFSET + (absolute_address - ExternalRAM::START_ADDRESS)] = data;
			return true;
		}
	}

	return false;
}

bool MemoryBankController_2::RamRomBankNumberAction(const data_t data)
{
	SANITY(this->_inner_mbc.RomUpperBankNumberAction(data), "Failed setting upper bank");

	if (this->_ram_enable)
	{
		this->_selected_ram_bank = (data & 0x03);
	}

	return true;
}
}