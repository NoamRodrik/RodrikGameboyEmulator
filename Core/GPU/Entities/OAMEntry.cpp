/**
 * @file		OAMEntry.cpp
 * @author		Noam Rodrik
 * @description Implementation to parsing an OAM entry from the OAM RAM.
 */
#include <Core/GPU/Registers/LCDC_Control.h>
#include <Core/GPU/Entities/OAMEntry.h>
#include <Core/Bus/Devices/OAMRAMDevice.h>

using namespace API;

namespace Core
{
OAMEntry::OAMEntry(IMemoryDeviceAccess& memory_accessor, std::size_t index) : _memory_accessor{memory_accessor}
{
	API::data_t attributes{0x00};
	SANITY(memory_accessor.Read(OAMRAMDevice::START_ADDRESS + index * OAM_ENTRY_SIZE + X_POSITION_INDEX, this->_x),
		   "Failed reading X position index from OAM entry: %llu.", index);
	this->_x -= PixelRow::PIXEL_COUNT;
	SANITY(memory_accessor.Read(OAMRAMDevice::START_ADDRESS + index * OAM_ENTRY_SIZE + Y_POSITION_INDEX, this->_y),
		   "Failed reading Y position index from OAM entry: %llu.", index);
	this->_y -= 2 * PixelRow::PIXEL_COUNT;
	SANITY(memory_accessor.Read(OAMRAMDevice::START_ADDRESS + index * OAM_ENTRY_SIZE + TILE_NUMBER_INDEX, this->_tile_index),
		   "Failed reading tile number index from OAM entry: %llu.", index);
	SANITY(memory_accessor.Read(OAMRAMDevice::START_ADDRESS + index * OAM_ENTRY_SIZE + ATTRIBUTES_INDEX, attributes),
		"Failed reading tile number index from OAM entry: %llu.", index);

	const OAMAttributes OAM_ATTRIBUTES{attributes};
	this->_palette = static_cast<Palette>(OAM_ATTRIBUTES.palette);
	this->_priority = static_cast<Priority>(OAM_ATTRIBUTES.priority);
	this->_x_flip = static_cast<Flip>(OAM_ATTRIBUTES.x_flip);
	this->_y_flip = static_cast<Flip>(OAM_ATTRIBUTES.y_flip);
}

PixelRow OAMEntry::GetSpritePixelRow(std::size_t y) const
{
	// Deciding which line to fetch from the sprite.
	y -= this->_y;

	const bool WIDE_SPRITES{static_cast<LCDC_Control::Control>(LCDC_Control{}).AreSpritesWide()};
	API::data_t tile_index = this->_tile_index & (0xFE | !WIDE_SPRITES);
	tile_index += !WIDE_SPRITES ? 0 : (this->_y_flip == Flip::NO && y >= PixelRow::PIXEL_COUNT) || (this->_y_flip == Flip::YES && y < PixelRow::PIXEL_COUNT);

	if (this->_y_flip == Flip::YES)
	{
		y = PixelRow::PIXEL_COUNT + WIDE_SPRITES * PixelRow::PIXEL_COUNT - y;
	}

	// Fetching the sprite tile address.
	const API::address_t SPRITE_TILE_ADDRESS = TILE_SET_BANK_0_OFFSET + (tile_index * 2 * PixelRow::PIXEL_COUNT) + 2 * y;
	API::data_t lower{0x00};
	API::data_t upper{0x00};

	// Reading lower and upper data of the pixel_row.
	SANITY(this->_memory_accessor.Read(SPRITE_TILE_ADDRESS, upper), "Failed reading lower pixel row from sprite");
	SANITY(this->_memory_accessor.Read(SPRITE_TILE_ADDRESS + 1, lower), "Failed reading upper pixel row from sprite");
	PixelRow pixel_row{upper, lower};

	// Checking if X is flipped.
	if (this->_x_flip == Flip::YES)
	{
		pixel_row.Flip();
	}

	return pixel_row;
}

const bool OAMEntry::IsInScanline(std::size_t y) const
{
	const bool WIDE_SPRITES{static_cast<LCDC_Control::Control>(LCDC_Control{}).AreSpritesWide()};
	return ((y >= this->_y) && (y < (this->_y + PixelRow::PIXEL_COUNT + static_cast<API::data_t>(WIDE_SPRITES) * PixelRow::PIXEL_COUNT)));
}
} // Core
