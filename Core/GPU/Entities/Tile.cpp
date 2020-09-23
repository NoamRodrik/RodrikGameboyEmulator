/**
 * @file		Tile.cpp
 * @author		Noam Rodrik
 * @description Main logic of tiles in ppu
 */
#include <Core/GPU/Entities/Tile.h>
#include <Core/CPU/Processor.h>
#include <API/Definitions.h>
#include <Tools/Tools.h>

using namespace Core;
using namespace API;

Tile::Tile(address_t start_address)
{
	SANITY(this->LoadTile(start_address), "Failed loading tile!");
}

bool Tile::LoadTile(API::address_t start_address)
{
	RET_FALSE_IF_FAIL(start_address < MEMORY_SIZE - Tile::HEIGHT_PIXELS, "Loading tiles out of memory range");

	for (auto&& pixel : this->_pixels)
	{
		RET_FALSE_IF_FAIL(pixel.LoadPixelRow(start_address++), "Failed loading pixel row %04X", start_address - 1);
	}

	return true;
}

const PixelRow& Tile::GetPixelRow(std::size_t index) const
{
	SANITY(index < sizeof(this->_pixels), "Got an invalid index size!");
	return this->_pixels[index];
}