/**
 * @file		Definitions.h
 * @author		Noam Rodrik
 * @description LR35902 PPU definitions
 */
#ifndef __LR35902_DEFINITIONS_H__
#define __LR35902_DEFINITIONS_H__

#include <API/Definitions.h>
#include <Tools/Tools.h>

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

/**
 * The source in which the pixel is written from.
 */
enum class PixelSource : API::data_t
{
	BGP = 0x00,
	OBP0 = 0x01,
	OBP1 = 0x02,
	WIN = 0x03
};

/**
 * The state of the PPU.
 */
enum class PPUState
{
	H_BLANK = 0x00,
	V_BLANK = 0x01,
	OAM_SEARCH = 0x02,
	PIXEL_RENDER = 0x03
};

static constexpr const char ENGINE_WINDOW_NAME[]{"RodrikGameBoyEmulator"};
static constexpr size_t     BACKGROUND_MAP_SIZE{Tools::SlotsToBytes(10)};
static constexpr size_t     SCREEN_WIDTH_PIXELS{160};
static constexpr size_t     SCREEN_HEIGHT_PIXELS{144};

//#define GREEN_DISPLAY
#ifdef GREEN_DISPLAY
static constexpr uint8_t    WHITE_PIXEL[]{143, 144, 85};
static constexpr uint8_t    BLACK_PIXEL[]{32, 55, 50};
static constexpr uint8_t    LIGHT_GREY_PIXEL[]{81, 113, 88};
static constexpr uint8_t    DARK_GREY_PIXEL[]{55, 85, 76};
#else
static constexpr uint8_t    WHITE_PIXEL[]{255, 255, 255};
static constexpr uint8_t    BLACK_PIXEL[]{0, 0, 0};
static constexpr uint8_t    LIGHT_GREY_PIXEL[]{221, 180, 180};
static constexpr uint8_t    DARK_GREY_PIXEL[]{97, 79, 76};
#endif

}

#endif // __LR35902_DEFINITIONS_H__