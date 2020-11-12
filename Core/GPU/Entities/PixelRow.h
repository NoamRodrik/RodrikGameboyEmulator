/**
 * @file		PixelRow.h
 * @author		Noam Rodrik
 * @description LR35902 pixel row object for the PPU
 */
#ifndef __LR35902_PIXEL_ROW_H__
#define __LR35902_PIXEL_ROW_H__

#include <Core/GPU/Definitions.h>
#include <API/Definitions.h>
#include <Tools/Tools.h>

namespace Core
{
/**
 * Rows are described in this form:
 * Each pixel has two bits, (ranging from 0x00 to 0x03).
 * 
 * Upper: 0|0|0|0|0|0|1|0
 * Lower: 1|1|1|1|1|1|1|1
 * 
 * Each column is a bit, but data is read as a row.
 * Thus there are 8 pixels in one row.
 */
class [[nodiscard]] PixelRow
{
public:
	constexpr PixelRow() = default;
	constexpr PixelRow(API::data_t upper, API::data_t lower) : _upper{upper},
															   _lower{lower} {}

	~PixelRow() = default;

	constexpr void SetUpper(API::data_t upper)
	{
		this->_upper = upper;
	}

	constexpr void SetLower(API::data_t lower)
	{
		this->_lower = lower;
	}

public:
	[[nodiscard]] constexpr auto StealTopColor()
	{
		const auto COLOR = PaletteColor((static_cast<uint8_t>(Tools::IsBitSet(this->_upper, TOP_COLOR_INDEX)) << 1) |
					  				    (static_cast<uint8_t>(Tools::IsBitSet(this->_lower, TOP_COLOR_INDEX))));
		this->_lower <<= 1;
		this->_upper <<= 1;
		return COLOR;
	}

	constexpr void SetBottomColor(const PaletteColor pixel_color)
	{
		Tools::MutateBitByCondition(static_cast<API::data_t>(pixel_color) & 0x02, this->_upper, 0);
		Tools::MutateBitByCondition(static_cast<API::data_t>(pixel_color) & 0x01, this->_lower, 0);
	}

	constexpr void SetTopColor(const PaletteColor pixel_color)
	{
		Tools::MutateBitByCondition(static_cast<API::data_t>(pixel_color) & 0x02, this->_upper, 7);
		Tools::MutateBitByCondition(static_cast<API::data_t>(pixel_color) & 0x01, this->_lower, 7);
	}

	constexpr void ShiftRightPixels(const std::size_t count, const PaletteColor color)
	{
		for (std::size_t index = 0; index < count; ++index)
		{
			this->_upper >>= 1;
			this->_lower >>= 1;
			SetTopColor(color);
		}
	}

	constexpr void ShiftLeftPixels(const std::size_t count, const PaletteColor color)
	{
		for (std::size_t index = 0; index < count; ++index)
		{
			this->_upper <<= 1;
			this->_lower <<= 1;
			SetBottomColor(color);
		}
	}

	/**
	 * Flips the row by the X axis.
	 */
	constexpr void Flip()
	{
		this->_lower = Tools::ReverseByte(this->_lower);
		this->_upper = Tools::ReverseByte(this->_upper);
	}

	constexpr PixelColor GetColorAtIndex(const std::size_t index) const
	{
		SANITY(index < PIXEL_COUNT, "Invalid index");
		return static_cast<PixelColor>(Tools::IsBitSet(this->_upper, index) << 1 | Tools::IsBitSet(this->_lower, index));
	}

	constexpr void SetColorAtIndex(const std::size_t index, const PixelColor color)
	{
		Tools::MutateBitByCondition(static_cast<size_t>(color) & 0b10, this->_upper, index);
		Tools::MutateBitByCondition(static_cast<size_t>(color) & 0b01, this->_lower, index);
	}

public:
	static constexpr API::data_t TOP_COLOR_INDEX{7};
	static constexpr API::data_t PIXEL_COUNT{8};

private:
	API::data_t _upper{0};
	API::data_t _lower{0};
};
} // Core

#endif // __LR35902_PIXEL_ROW_H__