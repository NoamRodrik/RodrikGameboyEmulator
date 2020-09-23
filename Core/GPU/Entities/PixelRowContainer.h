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
	const bool LoadPixelRow(const API::address_t address, const PixelSource pixel_source = PixelSource::BGP)
	{
		// Reset data.
		this->ResetPixelRowIndex();
		this->_pixel_source = pixel_source;
		return this->_current_pixel_row.LoadPixelRow(address);
	}

	const bool LoadPixelRow(const PixelRow& pixel_row, const PixelSource pixel_source = PixelSource::BGP)
	{
		// Reset data.
		this->ResetPixelRowIndex();
		this->_pixel_source = pixel_source;
		this->_current_pixel_row = pixel_row;
		return true;
	}

public:
	/**
	 * No pixels left in the pixel container.
	 */
	const bool IsEmpty() const
	{
		return this->_pixel_row_index > PixelRow::PIXEL_COUNT;
	}

	/**
	 * Fetching left-most pixel from the pixel row.
	 */
	const PaletteColor GetNextPixel()
	{
		SANITY(this->_pixel_row_index >= 1 && this->_pixel_row_index <= PixelRow::PIXEL_COUNT,
			   "Got invalid values for pixel row index");
		return this->_current_pixel_row.GetColorByIndex(PixelRow::PIXEL_COUNT - this->_pixel_row_index++);
	}

	/**
	 * Returns from where the pixel was taken (Background, OAM....).
	 */
	const PixelSource GetSource() const
	{
		return this->_pixel_source;
	}

private:
	void ResetPixelRowIndex()
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