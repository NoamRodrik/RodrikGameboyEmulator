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
class PixelRow
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
	/**
	 * Index is zero based!
	 */
	constexpr auto GetColorByIndex(uint8_t index) const
	{
		SANITY(index <= 7, "Can't get an index higher than bits");
		return PaletteColor{(static_cast<uint8_t>(Tools::IsBitSet(this->_upper, index)) << 1) | (static_cast<uint8_t>(Tools::IsBitSet(this->_lower, index)))};
	}

	/**
	 * Index is zero based!
	 */
	constexpr void SetColorToIndex(PaletteColor pixel_color, uint8_t index)
	{
		Tools::ClearBit(this->_upper, index);
		Tools::ClearBit(this->_lower, index);
		Tools::MutateBitByCondition(static_cast<API::data_t>(pixel_color) & 0x02, this->_upper, index);
		Tools::MutateBitByCondition(static_cast<API::data_t>(pixel_color) & 0x01, this->_lower, index);
	}

public:
	static constexpr auto PIXEL_COUNT{8};

private:
	API::data_t _upper{0};
	API::data_t _lower{0};
};
}

#endif // __LR35902_PIXEL_ROW_H__