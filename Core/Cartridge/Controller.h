/**
 * @file		MBCController.h
 * @author		Noam Rodrik
 * @description Decides which MBC the game should use.
 */
#ifndef __MBC_CONTROLLER_H__
#define __MBC_CONTROLLER_H__

#include <API/Memory/Device/IMemoryDeviceAccess.h>
#include <API/Cartridge/IMemoryBankController.h>
#include <API/Definitions.h>
#include <memory>
#include <array>

namespace Core
{
/**
 * Serves as an abstraction between the MBC type and the MBC operations.
 * Must call UpdateMBC before usage (otherwise will use the wrong MBC).
 */
class MBCController : public API::AMemoryBankController
{
public:
	MBCController(API::IMemoryDeviceAccess& memory_accessor, std::shared_ptr<API::ILoader> loader);
	virtual ~MBCController() = default;

public:
	/**
	 * Checks if the MBC has changed. If it has, changes the currently
	 * used MBC.
	 *
	 * Returns true if exists such MBC.
	 */
	bool UpdateMBC();

public:
	virtual API::CartridgeHeader::CartridgeType Type() const override;
	virtual size_t BankSize() const override;
	virtual void LoadMBC() override;
	virtual bool Read(const API::address_t absolute_address, API::data_t& result) const override;
	virtual bool Write(const API::address_t absolute_address, const API::data_t data) override;
	virtual bool WriteDirectly(const API::address_t absolute_address, const API::data_t data) override;

private:
	void Setup();

private:
	std::array<std::unique_ptr<API::IMemoryBankController>, API::MEMORY_BANK_CONTROLLERS_AMOUNT> _mbcs{};
	std::size_t _chosen_mbc{0};
};
} // Core

#endif // __MBC_CONTROLLER_H__