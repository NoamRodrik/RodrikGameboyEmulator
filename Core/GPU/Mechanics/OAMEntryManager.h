/**
 * @file		OAMEntryManager.h
 * @author		Noam Rodrik
 * @description Completely represents OAM entries and their complications.
 */
#ifndef __LR35902_OAM_ENTRY_MANAGER_H__
#define __LR35902_OAM_ENTRY_MANAGER_H__

#include <API/Memory/Device/IMemoryDeviceAccess.h>
#include <Core/GPU/Entities/OAMEntry.h>
#include <Core/Bus/Devices/OAMRAMDevice.h>
#include <memory>
#include <array>

namespace Core
{
class OAMEntryManager
{
public:
	OAMEntryManager(API::IMemoryDeviceAccess& memory_accessor) : _memory_accessor{memory_accessor}
	{
		this->LoadSprites();
	}

public:
	using sprites_in_line_t = std::array<std::shared_ptr<OAMEntry>, AMOUNT_OF_SPRITES_PER_LINE>;

public:
	void LoadSprites();
	[[nodiscard]] sprites_in_line_t GetSpritesInLine(std::size_t y);

private:
	[[nodiscard]] OAMEntry GetEntry(const std::size_t index) const;

private:
	std::array<std::shared_ptr<OAMEntry>, AMOUNT_OF_SPRITES> _sprites{nullptr};
	API::IMemoryDeviceAccess&								 _memory_accessor;
};
} // Core

#endif // __LR35902_OAM_ENTRY_MANAGER_H__