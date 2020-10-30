/**
 * @file		OAMEntryManager.cpp
 * @author		Noam Rodrik
 * @description Implementation to the OAM entry manager.
 */
#include <Core/GPU/Mechanics/OAMEntryManager.h>

using namespace API;

namespace Core
{
void OAMEntryManager::LoadSprites()
{
	// Loading all sprites again.
	for (std::size_t index = 0; index < this->_sprites.size(); ++index)
	{
		this->_sprites[index] = std::make_shared<OAMEntry>(this->GetEntry(index));
	}
}

OAMEntryManager::sprites_in_line_t OAMEntryManager::GetSpritesInLine(std::size_t y)
{
	std::size_t sprite_array_index{0x00};
	sprites_in_line_t sprites{};
	sprites.fill(nullptr);

	for (auto&& sprite : this->_sprites)
	{
		if (sprite_array_index == AMOUNT_OF_SPRITES_PER_LINE)
		{
			break;
		}

		if (sprite->IsInScanline(y))
		{
			sprites[sprite_array_index++] = sprite;
		}
	}

	return sprites;
}

OAMEntry OAMEntryManager::GetEntry(const std::size_t index) const
{
	return {this->_memory_accessor, index};
}
} // Core
