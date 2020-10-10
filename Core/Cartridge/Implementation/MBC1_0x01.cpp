/**
 * @file		MBC1_0x01.cpp
 * @author		Noam Rodrik
 * @description Main logic of MBC1
 * @note        According to: https://retrocomputing.stackexchange.com/questions/11732/how-does-the-gameboys-memory-bank-switching-work
 */
#include "MBC1_0x01.h"

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
} 

size_t MemoryBankController_1::BankSize() const
{
	return MemoryBankController_1::BANK_SIZE;
}

bool MemoryBankController_1::Read(const API::address_t absolute_address, API::data_t& result) const
{
	static CartridgeRAM* CRAM{static_cast<CartridgeRAM*>(Processor::GetInstance().GetMemory().GetDeviceAtAddress(CartridgeRAM::START_ADDRESS))};

	if (!CRAM->IsBootCovered() && absolute_address < CartridgeRAM::BOOT_END_ADDRESS)
	{
		// Don't intercept.
		return false;
	}

	if (absolute_address >= CartridgeRAM::START_ADDRESS &&
		absolute_address <= CartridgeRAM::END_ADDRESS)
	{
		if (absolute_address < ADDITIONAL_ROM_BANKS_OFFSET)
		{
			const size_t BASE_BANK_ZERO_OFFSET{Tools::BytesInROMBanks(this->_mode == Mode::RAM_MODE ? this->_selected_zero_bank : 0x00)};
			SANITY(BASE_BANK_ZERO_OFFSET + absolute_address < this->_rom_memory.MEMORY_SIZE,
				   "Accessed invalid ROM memory");
			result = this->_rom_memory[BASE_BANK_ZERO_OFFSET + absolute_address];
		}
		else
		{
			const size_t BASE_BANK_OFFSET{Tools::BytesInROMBanks(this->_selected_rom_bank)};
			SANITY(BASE_BANK_OFFSET + (absolute_address - ADDITIONAL_ROM_BANKS_OFFSET) < this->_rom_memory.MEMORY_SIZE, 
				   "Accessed invalid ROM memory");
			result = this->_rom_memory[BASE_BANK_OFFSET + (absolute_address - ADDITIONAL_ROM_BANKS_OFFSET)];
		}

		// Intercepted.
		return true;
	}

	// We don't need to intercept reads.
	return false;
}

bool MemoryBankController_1::WriteDirectly(const API::address_t absolute_address, const API::data_t data)
{
	// For MBC, write directly is just write....
	return this->Write(absolute_address, data);
}

bool MemoryBankController_1::Write(const API::address_t absolute_address, const API::data_t data)
{
	if (absolute_address >= MemoryBankController_1::ROM_LOWER_BANK_NUMBER_START &&
	    absolute_address <= MemoryBankController_1::ROM_LOWER_BANK_NUMBER_END)
	{
		SANITY(this->RomLowerBankNumberAction(data), "Failed setting rom lower bank");
		return true;
	}
	else if (absolute_address >= MemoryBankController_1::ROM_UPPER_BANK_NUMBER_START &&
		     absolute_address <= MemoryBankController_1::ROM_UPPER_BANK_NUMBER_END)
	{
		SANITY(this->RomUpperBankNumberAction(data), "Failed setting rom upper bank");
		return true;
	}
	else if (absolute_address >= MemoryBankController_1::RAM_ROM_MODE_SELECT_START &&
		     absolute_address <= MemoryBankController_1::RAM_ROM_MODE_SELECT_END)
	{
		// We need to check what mode is selected, then we can intercept the request.
		this->_mode = static_cast<Mode>(data & 0x01);
		return true;
	}

	return false;
}

const bool MemoryBankController_1::RomUpperBankNumberAction(const data_t data)
{
	CartridgeHeader header{this->_rom_memory.GetMemoryPointer()};

	// Select the upper 2 bits of the bank number.
	// Selecting a new ROM bank.
	API::data_t sized_data{0x00};
	if (static_cast<size_t>(header.ROMSize()) >= static_cast<size_t>(CartridgeHeader::ROMSizeValue::_1000_KB))
	{
		sized_data = static_cast<size_t>(header.ROMSize()) < static_cast<size_t>(CartridgeHeader::ROMSizeValue::_2000_KB) ?
			(data & 0b01) << 5:
			(data & 0b11) << 5;
	}

	this->_selected_rom_bank = sized_data | (this->_selected_rom_bank & 0x1F);

	if (this->_mode == Mode::RAM_MODE)
	{
		this->_selected_zero_bank = 0;

		if (static_cast<size_t>(header.ROMSize()) >= static_cast<size_t>(CartridgeHeader::ROMSizeValue::_1000_KB))
		{
			this->_selected_zero_bank = static_cast<size_t>(header.ROMSize()) < static_cast<size_t>(CartridgeHeader::ROMSizeValue::_2000_KB) ?
				(data & 0b01) << 5 :
				(data & 0b11) << 5;
		}
	}

	return true;
}

const bool MemoryBankController_1::RomLowerBankNumberAction(const data_t data)
{
	CartridgeHeader header{this->_rom_memory.GetMemoryPointer()};

	API::data_t mask{0b11111};
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

	// Select the lower 5 bits (masked) of the bank number.
	this->_selected_rom_bank = (this->_selected_rom_bank & 0x60) | ((data & 0b11111) == 0x0 ? 0x1 : (data & mask));

	return true;
}
} // Core