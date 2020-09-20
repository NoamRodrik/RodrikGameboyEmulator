/**
 * @file		PaletteMap.h
 * @author		Noam Rodrik
 * @description LR35902 maps the chosen palette with their corresponding colors.
 */
#ifndef __LR35902_PALETTE_MAP_H__
#define __LR35902_PALETTE_MAP_H__

#include <Core/GPU/Registers/BGP.h>
#include <Core/GPU/Definitions.h>

namespace Core
{
class PaletteMap
{
public:
	static const PixelColor ColorOf(const PaletteColor color)
	{
		// Get the left bits of the chosen color index.
		return static_cast<PixelColor>(((static_cast<API::data_t>(color) * 2) >> static_cast<API::data_t>(BGP{})) & 0x03);
	}
};
}

#endif // __LR35902_PALETTE_MAP_H__