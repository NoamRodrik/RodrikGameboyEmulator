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
class [[nodiscard]] PixelRowContainer
{
public:
	constexpr PixelRowContainer() = default;
	~PixelRowContainer() = default;

public:
	constexpr void SetUpper(const API::data_t upper)
	{
		this->_current_pixel_row.SetUpper(upper);
	}

	constexpr void SetLower(const API::data_t lower)
	{
		this->_current_pixel_row.SetLower(lower);
	}

	constexpr void Initialize(const PixelSource pixel_source = PixelSource::BGP)
	{
		this->_pixel_source = pixel_source;
		this->_pixel_row_index = UNUSED_PIXEL_ROW_INDEX_COUNT;
	}

	constexpr void SetPixelRow(const PixelRow pixel_row)
	{
		this->_current_pixel_row = pixel_row;
	}

	/**
	 * No pixels left in the pixel container.
	 */
	[[nodiscard]] constexpr bool IsEmpty() const
	{
		return this->_pixel_row_index > PixelRow::PIXEL_COUNT;
	}

	/**
	 * Fetching left-most pixel from the pixel row.
	 */
	[[nodiscard]] constexpr PaletteColor GetNextPixel()
	{
		SANITY(this->_pixel_row_index >= UNUSED_PIXEL_ROW_INDEX_COUNT && this->_pixel_row_index <= PixelRow::PIXEL_COUNT,
			"Got invalid values for pixel row index");
		this->_pixel_row_index += 1;
		return this->_current_pixel_row.StealTopColor();
	}

	constexpr void SetLastPixel(PaletteColor pixel_color)
	{
		this->_pixel_row_index -= 1;
		this->_current_pixel_row.SetBottomColor(pixel_color);
	}

	/**
	 * Returns from where the pixel was taken (Background, OAM....).
	 */
	[[nodiscard]] constexpr PixelSource GetSource() const
	{
		return this->_pixel_source;
	}

	constexpr void Clear()
	{
		this->_pixel_row_index = EMPTY_PIXEL_ROW_INDEX_COUNT;
		this->_pixel_source = PixelSource::BGP;
		this->SetPixelRow(PixelRow{});
	}

	[[nodiscard]] constexpr std::size_t EmptyBitsAmount()
	{
		return PixelRow::PIXEL_COUNT - (EMPTY_PIXEL_ROW_INDEX_COUNT - this->_pixel_row_index);
	}

private:
	static constexpr std::size_t UNUSED_PIXEL_ROW_INDEX_COUNT{1};
	static constexpr std::size_t EMPTY_PIXEL_ROW_INDEX_COUNT{9};

private:
	PixelRow    _current_pixel_row{};
	std::size_t _pixel_row_index{EMPTY_PIXEL_ROW_INDEX_COUNT};
	PixelSource _pixel_source{PixelSource::BGP};
};
} // Core

#endif // __LR35902_PIXEL_ROW_CONTAINER_H__