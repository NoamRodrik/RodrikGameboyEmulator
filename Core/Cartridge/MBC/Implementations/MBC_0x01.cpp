/**
 * @file		MBC_0x01.cpp
 * @author		Noam Rodrik
 * @description Main logic of MBC1
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
	DeviceTools::Map(this->m_memory.GetMemoryPointer(), Tools::BytesInBanks(1), CartridgeRAM::START_ADDRESS);
} 

size_t MemoryBankController_1::BankSize() const
{
	return MemoryBankController_1::BANK_SIZE;
}

bool MemoryBankController_1::Read(const API::address_t absolute_address, API::data_t& result) const
{
	Message("TODO");
	return false;
}

bool MemoryBankController_1::Write(const API::address_t absolute_address, const API::data_t data)
{
	Message("TODO");
	return false;
}
}