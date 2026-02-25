/**
 * @file		OAMEntry.cpp
 * @author		Noam Rodrik
 * @description Implementation to parsing an OAM entry from the OAM RAM.
 */
#include <Core/GPU/Registers/LCDC_Control.h>
#include <Core/GPU/Entities/OAMEntry.h>
#include <Core/Bus/Devices/OAMRAMDevice.h>
#include <Core/GPU/Registers/SCY.h>
#include <Core/GPU/Registers/SCX.h>

using namespace API;

namespace Core
{
OAMEntry::OAMEntry(IMemoryDeviceAccess& memory_accessor, int32_t index) : _memory_accessor{memory_accessor}, _id{index}
{
	API::data_t attributes{0x00};
	SANITY(memory_accessor.Read(OAMRAMDevice::START_ADDRESS + index * OAM_ENTRY_SIZE + X_POSITION_INDEX, this->_x),
		   "Failed reading X position index from OAM entry: %llu.", index);
	SANITY(memory_accessor.Read(OAMRAMDevice::START_ADDRESS + index * OAM_ENTRY_SIZE + Y_POSITION_INDEX, this->_y),
		   "Failed reading Y position index from OAM entry: %llu.", index);
	SANITY(memory_accessor.Read(OAMRAMDevice::START_ADDRESS + index * OAM_ENTRY_SIZE + TILE_NUMBER_INDEX, this->_tile_index),
		   "Failed reading tile number index from OAM entry: %llu.", index);
	SANITY(memory_accessor.Read(OAMRAMDevice::START_ADDRESS + index * OAM_ENTRY_SIZE + ATTRIBUTES_INDEX, attributes),
		"Failed reading tile number index from OAM entry: %llu.", index);

	const OAMAttributes OAM_ATTRIBUTES{attributes};
	this->_palette = static_cast<Palette>(OAM_ATTRIBUTES.palette);
	this->_priority = static_cast<Priority>(OAM_ATTRIBUTES.priority);
	this->_x_flip = static_cast<Flip>(OAM_ATTRIBUTES.x_flip);
	this->_y_flip = static_cast<Flip>(OAM_ATTRIBUTES.y_flip);
	// OAM Y is screen_y + 16. Sprites live in screen space; SCY scrolls the background only.
}

PixelRow OAMEntry::GetSpritePixelRow(std::size_t y) const
{
	const bool WIDE_SPRITES{static_cast<LCDC_Control::Control>(LCDC_Control{}).AreSpritesWide()};
	const API::data_t TILE_INDEX = this->_tile_index & (0xFE | !WIDE_SPRITES);

	// Deciding which line to fetch from the sprite.
	// Use signed arithmetic to handle sprites partially above the top of screen
	// (e.g. OAM Y = 12 → _y = 12, screen row 0 → row 4 of the sprite).
	const std::size_t SPRITE_HEIGHT = WIDE_SPRITES ? 2 * PixelRow::PIXEL_COUNT : PixelRow::PIXEL_COUNT;
	y = static_cast<std::size_t>(static_cast<int32_t>(y) - static_cast<int32_t>(this->_y) + 16);
	if (this->_y_flip == Flip::YES)
	{
		y = (SPRITE_HEIGHT - 1) - y;
	}

	// Fetching the sprite tile address.
	const API::address_t SPRITE_TILE_ADDRESS = TILE_SET_BANK_0_OFFSET + (TILE_INDEX * 2 * PixelRow::PIXEL_COUNT) + 2 * y;
	API::data_t lower{0x00};
	API::data_t upper{0x00};

	// Byte 0 = LSB of each pixel's 2-bit color, Byte 1 = MSB.
	// PixelRow expects _upper=MSB, _lower=LSB, so pass them in the correct order.
	SANITY(this->_memory_accessor.Read(SPRITE_TILE_ADDRESS, lower), "Failed reading lower pixel row from sprite");
	SANITY(this->_memory_accessor.Read(SPRITE_TILE_ADDRESS + 1, upper), "Failed reading upper pixel row from sprite");
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
	const int32_t sprite_top = static_cast<int32_t>(this->_y) - 16;
	return this->_x != 0 &&
		(static_cast<int32_t>(y) >= sprite_top &&
		 static_cast<int32_t>(y) < sprite_top + (WIDE_SPRITES ? 16 : 8));
}
} // Core
