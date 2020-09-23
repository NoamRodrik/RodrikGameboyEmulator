/**
 * @file		Tile.h
 * @author		Noam Rodrik
 * @description LR35902 tile object for the PPU
 */
#ifndef __LR35902_TILE_H__
#define __LR35902_TILE_H__

#include <Core/GPU/Entities/PixelRow.h>
#include <API/Definitions.h>
#include <cstddef>

namespace Core
{
class Tile
{
public:
	constexpr Tile() = default;
	Tile(API::address_t start_address);
	~Tile() = default;

public:
	bool LoadTile(API::address_t start_address);
	const PixelRow& GetPixelRow(std::size_t index) const;

public:
	static constexpr std::size_t    HEIGHT_PIXELS{8};
	static constexpr API::address_t TILE_SIZE{sizeof(PixelRow) * HEIGHT_PIXELS};

private:
	// 8x8
	PixelRow _pixels[HEIGHT_PIXELS]{};
};
}

#endif // __LR35902_TILE_H__