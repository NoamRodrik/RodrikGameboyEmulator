/**
 * @file		Controller.cpp
 * @author		Noam Rodrik
 * @description Main logic of the mbc controller
 */
#include "Controller.h"

#include <Core/Cartridge/Implementations/ROM_0x00.h>
#include <Core/Cartridge/Implementations/MBC_0x01.h>
#include <API/Cartridge/Header.h>

using namespace API;

namespace Core
{
MBCController::MBCController(IMemoryDeviceAccess& memory_accessor, std::shared_ptr<API::ILoader> loader) :
	AMemoryBankController{memory_accessor, loader}
{
	std::fill(this->m_mbcs.begin(), this->m_mbcs.end(), nullptr);
	this->Setup();
}

void MBCController::Setup()
{
	this->m_mbcs[0] = std::make_unique<MemoryBankController_ROM>(this->m_memory_device, this->m_loader);
	this->m_mbcs[1] = std::make_unique<MemoryBankController_1>(this->m_memory_device, this->m_loader);
}

bool MBCController::UpdateMBC()
{
	CartridgeHeader loaded_header{this->m_memory_device};

	for (std::size_t current_index = 0; current_index < this->m_mbcs.size(); ++current_index)
	{
		if (this->m_mbcs[current_index].get() != nullptr &&
			this->m_mbcs[current_index]->Type() == loaded_header.MBC())
		{
			this->m_chosen_mbc = current_index;
			return true;
		}
	}

	return false;
}

CartridgeHeader::CartridgeType MBCController::Type() const
{
	SANITY(this->m_mbcs[this->m_chosen_mbc].get() != nullptr, "Failed fetching MBC");
	return this->m_mbcs[this->m_chosen_mbc]->Type();
}

size_t MBCController::BankSize() const
{
	SANITY(this->m_mbcs[this->m_chosen_mbc].get() != nullptr, "Failed fetching MBC");
	return this->m_mbcs[this->m_chosen_mbc]->BankSize();
}

void MBCController::LoadMBC()
{
	SANITY(this->UpdateMBC(), "Failed updating the MBC");
	SANITY(this->m_mbcs[this->m_chosen_mbc].get() != nullptr, "Failed fetching MBC");
	this->m_mbcs[this->m_chosen_mbc]->LoadMBC();
}

bool MBCController::Read(const API::address_t absolute_address, API::data_t& result) const
{
	SANITY(this->m_mbcs[this->m_chosen_mbc].get() != nullptr, "Failed fetching MBC");
	return this->m_mbcs[this->m_chosen_mbc]->Read(absolute_address, result);
}

bool MBCController::Write(const API::address_t absolute_address, const API::data_t data)
{
	SANITY(this->m_mbcs[this->m_chosen_mbc].get() != nullptr, "Failed fetching MBC");
	return this->m_mbcs[this->m_chosen_mbc]->Write(absolute_address, data);
}
}