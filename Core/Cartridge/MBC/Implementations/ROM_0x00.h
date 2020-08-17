/**
 * @file		ROM_0x00.h
 * @author		Noam Rodrik
 * @description Implementation of the most basic MBC.
 */
#ifndef __MBC_ROM_H__
#define __MBC_ROM_H__

#include <API/Cartridge/IMemoryBankController.h>

namespace Core
{
class MemoryBankController_ROM : public API::AMemoryBankController
{
public:
	using API::AMemoryBankController::AMemoryBankController;
	virtual ~MemoryBankController_ROM() = default;

public:
	virtual API::CartridgeHeader::CartridgeType Type() const override { return API::CartridgeHeader::CartridgeType::ROM; }
	virtual void LoadROMBankZero() override;
};
}

#endif // __MBC_ROM_H__