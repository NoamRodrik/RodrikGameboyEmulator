/**
 * @file		PixelRowContainer.h
 * @author		Noam Rodrik
 * @description LR35902 pixel row container for the FIFO.
 */
#ifndef __LR35902_PIXEL_ROW_CONTAINER_H__
#define __LR35902_PIXEL_ROW_CONTAINER_H__

#include <Core/GPU/Definitions.h>
#include <Core/CPU/Processor.h>
#include <API/Definitions.h>
#include <Tools/Tools.h>
#include "PixelRow.h"

namespace Core
{
class PixelRowContainer
{
public:
	constexpr PixelRowContainer() = default;
	~PixelRowContainer() = default;

public:
	constexpr bool LoadPixelRow(const PixelRow& pixel_row, const PixelSource pixel_source = PixelSource::BGP)
	{
		// Reset data.
		this->Clear();
		this->_pixel_source = pixel_source;
		this->_current_pixel_row = pixel_row;
		return true;
	}

	constexpr void SetUpper(API::data_t upper)
	{
		this->_current_pixel_row.SetUpper(upper);
	}

	constexpr void SetLower(API::data_t lower)
	{
		this->_current_pixel_row.SetLower(lower);
	}

public:
	/**
	 * No pixels left in the pixel container.
	 */
	constexpr bool IsEmpty() const
	{
		return this->_pixel_row_index > PixelRow::PIXEL_COUNT;
	}

	/**
	 * Fetching left-most pixel from the pixel row.
	 */
	constexpr PaletteColor GetNextPixel()
	{
		SANITY(this->_pixel_row_index >= 1 && this->_pixel_row_index <= PixelRow::PIXEL_COUNT,
			   "Got invalid values for pixel row index");
		return this->_current_pixel_row.GetColorByIndex(PixelRow::PIXEL_COUNT - this->_pixel_row_index++);
	}

	constexpr void SetLastPixel(PaletteColor pixel_color)
	{
		SANITY(this->_pixel_row_index > 1, "Setting too many pixels!");
		constexpr std::size_t RIGHT_BIT_INDEX{0};
		this->_current_pixel_row.SetColorToIndex(pixel_color, RIGHT_BIT_INDEX);
	}

	/**
	 * Returns from where the pixel was taken (Background, OAM....).
	 */
	constexpr PixelSource GetSource() const
	{
		return this->_pixel_source;
	}

	constexpr void Clear()
	{
		this->ResetPixelRowIndex();
		this->_pixel_source = PixelSource::BGP;
		this->_current_pixel_row = PixelRow{};
	}

private:
	/**
	 * Pixel row index is one based!
	 */
	constexpr void ResetPixelRowIndex()
	{
		this->_pixel_row_index = 1;
	}

private:
	PixelRow    _current_pixel_row{};
	std::size_t _pixel_row_index{1};
	PixelSource _pixel_source{PixelSource::BGP};
};
}

#endif // __LR35902_PIXEL_ROW_CONTAINER_H__