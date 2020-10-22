/**
 * @file		PaletteMap.h
 * @author		Noam Rodrik
 * @description LR35902 maps the chosen palette with their corresponding colors.
 */
#ifndef __LR35902_PALETTE_MAP_H__
#define __LR35902_PALETTE_MAP_H__

#include <Core/GPU/Registers/BGP.h>
#include <Core/GPU/Definitions.h>
#include <type_traits>

namespace Core
{
class PaletteMap
{
public:
	template <typename PALETTE_REGISTER>
	[[nodiscard]] static const PixelColor ColorOf(const PaletteColor color)
	{
		// Get the left bits of the chosen color index.
		return static_cast<PixelColor>(((PALETTE_REGISTER{} & (0b11 << (static_cast<API::data_t>(color) * 2))) >> (static_cast<API::data_t>(color) * 2)) & 0x03);
	}

	template <typename PALETTE_REGISTER>
	[[nodiscard]] static const PixelColor TransparentColor()
	{
		static_assert(!std::is_same_v<PALETTE_REGISTER, BGP>, "BGP doesn't have a transparent color");

		// Sprite data 00 is transparent.
		return static_cast<PixelColor>(PALETTE_REGISTER{} & 0x03);
	}
};
} // Core

#endif // __LR35902_PALETTE_MAP_H__