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
	constexpr void InitializeSource(const PixelSource pixel_source = PixelSource::BGP)
	{
		for (auto&& current_pixel_source : this->_pixel_source)
		{
			current_pixel_source = pixel_source;
		}
	}

	constexpr void SetPixelRow(const PixelRow pixel_row)
	{
		this->_current_pixel_row = pixel_row;
		this->_pixel_row_index = PixelRow::PIXEL_COUNT;
	}

	/**
	 * No pixels left in the pixel container.
	 */
	[[nodiscard]] constexpr bool IsEmpty() const
	{
		return this->_pixel_row_index == 0;
	}

	/**
	 * Fetching left-most pixel from the pixel row.
	 */
	[[nodiscard]] constexpr std::pair<PixelSource, PaletteColor> GetNextPixel()
	{
		SANITY(!this->IsEmpty() && this->_pixel_row_index <= PixelRow::PIXEL_COUNT,
			"Got invalid values for pixel row index");
		const auto FETCHED_PIXEL_SOURCE{this->_pixel_source[0]};

		// Shifting all the pixel sources to the left
		for (size_t pixel_source_index = 0; pixel_source_index < PixelRow::PIXEL_COUNT - 1; ++pixel_source_index)
		{
			this->_pixel_source[pixel_source_index] = this->_pixel_source[pixel_source_index + 1];
		}
		this->_pixel_source[PixelRow::PIXEL_COUNT - 1] = PixelSource::BGP;

		this->_pixel_row_index -= 1;
		return {FETCHED_PIXEL_SOURCE, this->_current_pixel_row.StealTopColor()};
	}

	constexpr void SetLastPixel(PixelSource source, PaletteColor pixel_color)
	{
		SANITY(this->EmptyBitsAmount() >= 1, "Too full");
		this->_pixel_source[this->_pixel_row_index] = source;
		this->_pixel_row_index += 1;
		this->_current_pixel_row.SetBottomColor(pixel_color);
	}

	constexpr void Clear()
	{
		this->SetPixelRow(PixelRow{});
		this->InitializeSource();
		this->_pixel_row_index = 0;
	}

	[[nodiscard]] constexpr std::size_t EmptyBitsAmount()
	{
		return PixelRow::PIXEL_COUNT - this->_pixel_row_index;
	}

private:
	PixelRow    _current_pixel_row{};
	std::size_t _pixel_row_index{0};
	PixelSource _pixel_source[PixelRow::PIXEL_COUNT]{PixelSource::BGP,
													 PixelSource::BGP,
													 PixelSource::BGP,
													 PixelSource::BGP,
													 PixelSource::BGP,
													 PixelSource::BGP,
													 PixelSource::BGP,
													 PixelSource::BGP};
};
} // Core

#endif // __LR35902_PIXEL_ROW_CONTAINER_H__