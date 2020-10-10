/**
 * @file		Controller.cpp
 * @author		Noam Rodrik
 * @description Main logic of the mbc controller
 */
#include "Controller.h"

#include <Core/Cartridge/Implementation/MBC1_0x03.h>
#include <Core/Cartridge/Implementation/MBC1_0x02.h>
#include <Core/Cartridge/Implementation/MBC1_0x01.h>
#include <Core/Cartridge/Implementation/ROM_0x00.h>
#include <Core/Bus/Devices/CartridgeRAM.h>
#include <API/Cartridge/Header.h>
#include <Core/CPU/Processor.h>

using namespace API;

namespace Core
{
MBCController::MBCController(IMemoryDeviceAccess& memory_accessor, std::shared_ptr<API::ILoader> loader) :
	AMemoryBankController{memory_accessor, loader}
{
	std::fill(this->_mbcs.begin(), this->_mbcs.end(), nullptr);
	this->Setup();

	// Load the first 8KB with the game data.
	this->_loader->Load(static_cast<CartridgeRAM*>(Processor::GetInstance().GetMemory().GetDeviceAtAddress(CartridgeRAM::START_ADDRESS))->GetMemoryPointer(),
						 static_cast<long>(Tools::BytesInROMBanks(1) / 2));

	SANITY(this->UpdateMBC(), "Failed loading MBC");
}

void MBCController::Setup()
{
	this->_mbcs[0] = std::make_unique<MemoryBankController_ROM>(this->_memory_device, this->_loader);
	this->_mbcs[1] = std::make_unique<MemoryBankController_1>(this->_memory_device, this->_loader);
	this->_mbcs[2] = std::make_unique<MemoryBankController_2>(this->_memory_device, this->_loader);
	this->_mbcs[3] = std::make_unique<MemoryBankController_3>(this->_memory_device, this->_loader);
}

const bool MBCController::UpdateMBC()
{
	CartridgeHeader loaded_header{&this->_memory_device};

	for (std::size_t current_index = 0; current_index < this->_mbcs.size(); ++current_index)
	{
		if (this->_mbcs[current_index].get() != nullptr &&
			this->_mbcs[current_index]->Type() == loaded_header.MBC())
		{
			this->_chosen_mbc = current_index;
			return true;
		}
	}
	
	MAIN_LOG("MBC does not exist: %u.", loaded_header.MBC());
	return false;
}

CartridgeHeader::CartridgeType MBCController::Type() const
{
	SANITY(this->_mbcs[this->_chosen_mbc].get() != nullptr, "Failed fetching MBC");
	return this->_mbcs[this->_chosen_mbc]->Type();
}

size_t MBCController::BankSize() const
{
	SANITY(this->_mbcs[this->_chosen_mbc].get() != nullptr, "Failed fetching MBC");
	return this->_mbcs[this->_chosen_mbc]->BankSize();
}

void MBCController::LoadMBC()
{
	SANITY(this->_mbcs[this->_chosen_mbc].get() != nullptr, "Failed fetching MBC");
	this->_mbcs[this->_chosen_mbc]->LoadMBC();
}

void MBCController::CloseMBC()
{
	SANITY(this->_mbcs[this->_chosen_mbc].get() != nullptr, "Failed fetching MBC");
	this->_mbcs[this->_chosen_mbc]->CloseMBC();
}

bool MBCController::Read(const API::address_t absolute_address, API::data_t& result) const
{
	SANITY(this->_mbcs[this->_chosen_mbc].get() != nullptr, "Failed fetching MBC");
	return this->_mbcs[this->_chosen_mbc]->Read(absolute_address, result);
}

bool MBCController::Write(const API::address_t absolute_address, const API::data_t data)
{
	SANITY(this->_mbcs[this->_chosen_mbc].get() != nullptr, "Failed fetching MBC");
	return this->_mbcs[this->_chosen_mbc]->Write(absolute_address, data);
}

bool MBCController::WriteDirectly(const API::address_t absolute_address, const API::data_t data)
{
	SANITY(this->_mbcs[this->_chosen_mbc].get() != nullptr, "Failed fetching MBC");
	return this->_mbcs[this->_chosen_mbc]->WriteDirectly(absolute_address, data);
}
} // Core