/**
 * @file		Definitions.h
 * @author		Noam Rodrik
 * @description LR35902 GPU definitions
 */
#ifndef __LR35902_GPU_DEFINITIONS_H__
#define __LR35902_GPU_DEFINITIONS_H__

#include <API/Definitions.h>
#include <Tools/Tools.h>

namespace Core
{
[[nodiscard]] static constexpr API::address_t GetWrappedAroundDistance(const std::size_t from, const std::size_t to)
{
	return (from + 0x100 - to) % 0x100;
}

/**
 * The original colors to choose from.
 */
enum class [[nodiscard]] PixelColor : API::data_t
{
	WHITE = 0x00,
	DARK_GREY = 0x01,
	LIGHT_GREY = 0x02,
	BLACK = 0x03
};

/**
 * The chosen colors as the palette.
 */
enum class [[nodiscard]] PaletteColor : API::data_t
{
	FIRST_PALETTE = 0x00,
	SECOND_PALETTE = 0x01,
	THIRD_PALETTE = 0x02,
	FOURTH_PALETTE = 0x03
};

/**
 * The state of the PPU.
 */
enum class [[nodiscard]] PPUState : API::data_t
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
static constexpr size_t     AMOUNT_OF_SPRITES{40};
static constexpr size_t     AMOUNT_OF_SPRITES_PER_LINE{10};
static constexpr int32_t    BGP_PIXEL{-1};

#ifdef GREEN_DISPLAY
static constexpr uint8_t    WHITE_PIXEL[]{143, 144, 85};
static constexpr uint8_t    BLACK_PIXEL[]{32, 55, 50};
static constexpr uint8_t    LIGHT_GREY_PIXEL[]{81, 113, 88};
static constexpr uint8_t    DARK_GREY_PIXEL[]{55, 85, 76};
#elif defined(BEAUTIFUL_DISPLAY)
static constexpr uint8_t    WHITE_PIXEL[]{255, 255, 255};
static constexpr uint8_t    BLACK_PIXEL[]{0, 0, 0};
static constexpr uint8_t    LIGHT_GREY_PIXEL[]{221, 180, 180};
static constexpr uint8_t    DARK_GREY_PIXEL[]{97, 79, 76};
#else
static constexpr uint8_t    WHITE_PIXEL[]{255, 255, 255};
static constexpr uint8_t    BLACK_PIXEL[]{0, 0, 0};
static constexpr uint8_t    LIGHT_GREY_PIXEL[]{203, 203, 203};
static constexpr uint8_t    DARK_GREY_PIXEL[]{149, 149, 149};
#endif

} // Core

#endif // __LR35902_GPU_DEFINITIONS_H__