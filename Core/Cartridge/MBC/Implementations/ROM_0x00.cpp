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
void MemoryBankController_ROM::LoadROMBankZero()
{
	// All that's needed is to load all the ROM, since that is what
	// this MBC does.
	this->m_loader.Load(static_cast<CartridgeRAM*>(Processor::GetInstance().GetMemory().GetDeviceAtAddress(CartridgeRAM::START_ADDRESS))->GetMemoryPointer(),
						static_cast<long>(CartridgeRAM::SIZE));
}
}