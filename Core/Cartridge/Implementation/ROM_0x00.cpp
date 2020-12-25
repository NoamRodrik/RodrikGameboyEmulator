/**
 * @file		ROM_0x00.cpp
 * @author		Noam Rodrik
 * @description Main logic of MBC ROM
 */
#include "ROM_0x00.h"

#include <Core/Bus/Devices/CartridgeROMDevice.h>
#include <Core/CPU/Processor.h>

using namespace API;

namespace Core
{
void MemoryBankController_ROM::LoadMBC()
{
	// All that's needed is to load all the ROM, since that is what
	// this MBC does.
	this->_loader->Load(static_cast<CartridgeROMDevice*>(Processor::GetInstance().GetMemory().GetDeviceAtAddress(CartridgeROMDevice::START_ADDRESS))->GetMemoryPointer(),
						 static_cast<long>(CartridgeROMDevice::SIZE));
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

bool MemoryBankController_ROM::WriteDirectly(const API::address_t absolute_address, const API::data_t data)
{
	// For MBC, write directly is just write....
	return this->Write(absolute_address, data);
}

size_t MemoryBankController_ROM::BankSize() const
{
	return MemoryBankController_ROM::BANK_SIZE;
}
} // Core