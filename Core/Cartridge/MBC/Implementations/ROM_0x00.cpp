/**
 * @file		ROM_0x00.cpp
 * @author		Noam Rodrik
 * @description Main logic of MBC ROM
 */
#include "ROM_0x00.h"

#include <Core/Bus/Devices/CartridgeRAM.h>
#include <Core/CPU/Processor.h>

using namespace API;

namespace Core
{
void MemoryBankController_ROM::LoadMBC()
{
	// All that's needed is to load all the ROM, since that is what
	// this MBC does.
	this->m_loader->Load(static_cast<CartridgeRAM*>(Processor::GetInstance().GetMemory().GetDeviceAtAddress(CartridgeRAM::START_ADDRESS))->GetMemoryPointer(),
						 static_cast<long>(CartridgeRAM::SIZE));
}

bool MemoryBankController_ROM::Read(const API::address_t, API::data_t&) const
{
	// No need to override.
	return false;
}

bool MemoryBankController_ROM::Write(const API::address_t, const API::data_t)
{
	// No need to override.
	return false;
}

size_t MemoryBankController_ROM::BankSize() const
{
	return MemoryBankController_ROM::BANK_SIZE;
}
}