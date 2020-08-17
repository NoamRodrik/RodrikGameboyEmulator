/**
 * @file		Controller.cpp
 * @author		Noam Rodrik
 * @description Main logic of the mbc controller
 */
#include "Controller.h"

#include <Core/Cartridge/MBC/Implementations/ROM_0x00.h>
#include <API/Cartridge/Header.h>

using namespace API;

namespace Core
{
MBCController::MBCController(IMemoryDeviceAccess& memory_accessor, API::ILoader& loader) :
	AMemoryBankController{memory_accessor, loader}
{
	std::fill(this->m_mbcs.begin(), this->m_mbcs.end(), nullptr);
	this->Setup();
}

void MBCController::Setup()
{
	this->m_mbcs[0] = std::make_unique<MemoryBankController_ROM>(this->m_memory_device, this->m_loader);
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

void MBCController::LoadROMBankZero()
{
	SANITY(this->m_mbcs[this->m_chosen_mbc].get() != nullptr, "Failed fetching MBC");
	this->m_mbcs[this->m_chosen_mbc]->LoadROMBankZero();
}

}