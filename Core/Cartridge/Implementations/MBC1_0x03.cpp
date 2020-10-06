/**
 * @file		MBC1_0x03.cpp
 * @author		Noam Rodrik
 * @description Main logic of MBC1 with RAM and BATTERY
 * @note        According to: https://retrocomputing.stackexchange.com/questions/11732/how-does-the-gameboys-memory-bank-switching-work
 */
#include "MBC1_0x03.h"

#include <Core/CPU/Processor.h>
#include <Tools/Tools.h>
#include <fstream>
#include <string>

using namespace API;

namespace Core
{
void MemoryBankController_3::LoadMBC()
{
	this->_inner_mbc.LoadMBC();
	CartridgeHeader cartridge_header{this->_inner_mbc._inner_mbc._rom_memory.GetMemoryPointer()};
	this->_filename = std::to_string(cartridge_header.GlobalChecksum()).append(FILE_POSTFIX);

	// Check that file_id.FILE_POSTFIX exists
	if (this->DoesFileExist())
	{
		// If it does, load that file to the RAM
		std::ifstream file_handler{this->_filename, std::ios::binary};
		file_handler.read(reinterpret_cast<char*>(this->_inner_mbc._ram_memory.GetMemoryPointer()), this->_inner_mbc._ram_memory.MEMORY_SIZE);
	}
	else
	{
		// Else, create such a file.
		std::ofstream file_handler{this->_filename, std::ios::binary};
		file_handler.write(reinterpret_cast<char*>(this->_inner_mbc._ram_memory.GetMemoryPointer()), this->_inner_mbc._ram_memory.MEMORY_SIZE);
	}
}

void MemoryBankController_3::CloseMBC()
{
	std::ofstream file_handler{this->_filename, std::ios::binary};
	file_handler.write(reinterpret_cast<char*>(this->_inner_mbc._ram_memory.GetMemoryPointer()), this->_inner_mbc._ram_memory.MEMORY_SIZE);
}

size_t MemoryBankController_3::BankSize() const
{
	return MemoryBankController_3::BANK_SIZE;
}

bool MemoryBankController_3::Read(const API::address_t absolute_address, API::data_t& result) const
{
	return this->_inner_mbc.Read(absolute_address, result);
}

bool MemoryBankController_3::WriteDirectly(const API::address_t absolute_address, const API::data_t data)
{
	return this->_inner_mbc.WriteDirectly(absolute_address, data);
}

bool MemoryBankController_3::Write(const API::address_t absolute_address, const API::data_t data)
{
	return this->_inner_mbc.Write(absolute_address, data);
}

const bool MemoryBankController_3::DoesFileExist() const
{
	std::ifstream file_handler{this->_filename};
	return file_handler.good();
}
} // Core