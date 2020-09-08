/**
 * @file		Definitions.h
 * @author		Noam Rodrik
 * @description LR35902 PPU definitions
 */
#ifndef __LR35902_DEFINITIONS_H__
#define __LR35902_DEFINITIONS_H__

#include <API/Definitions.h>

namespace Core
{
/**
 * The original colors to choose from.
 */
enum class PixelColor : API::data_t
{
	WHITE = 0x00,
	LIGHT_GREY = 0x01,
	DARK_GREY = 0x02,
	BLACK = 0x03
};

/**
 * The chosen colors as the palette.
 */
enum class PaletteColor : API::data_t
{
	FIRST_PALETTE = 0x00,
	SECOND_PALETTE = 0x01,
	THIRD_PALETTE = 0x02,
	FOURTH_PALETTE = 0x03
};
}

#endif // __LR35902_DEFINITIONS_H__