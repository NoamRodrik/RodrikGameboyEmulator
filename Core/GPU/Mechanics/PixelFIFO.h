/**
 * @file		PixelFIFO.h
 * @author		Noam Rodrik
 * @description LR35902 pixel FIFO header
 */
#ifndef __LR35902_PIXEL_FIFO_H__
#define __LR35902_PIXEL_FIFO_H__

#include <Core/GPU/Entities/PixelRowContainer.h>
#include <API/Definitions.h>
#include <Tools/Tools.h>

namespace Core
{
/**
 * The pixel FIFO works as a pixel priority mechanic.
 * It's in charge of all points in the PPU (from background, to sprites...).
 */
class PixelFIFO
{
public:
	constexpr PixelFIFO() = default;
	~PixelFIFO() = default;

public:
	constexpr auto FetchNextPixel()
	{
		SANITY(!this->_first_row.IsEmpty() || !this->_second_row.IsEmpty(),
			   "Both pixel containers in the FIFO are empty!");

		// Fetching from the first row if it isn't empty.
		if (!this->_first_row.IsEmpty())
		{
			const auto FETCHED_PALETTE_COLOR{this->_first_row.GetNextPixel()};
			
			// If the second row isn't empty, take the left bit from the second row
			// and apply it to the right bit of the first row.
			if (!this->_second_row.IsEmpty())
			{
				this->_first_row.SetLastPixel(this->_second_row.GetNextPixel());
			}

			return FETCHED_PALETTE_COLOR;
		}
		else
		{
			return this->_second_row.GetNextPixel();
		}
	}

	constexpr bool IsEmpty() const
	{
		return this->_first_row.IsEmpty() && this->_second_row.IsEmpty();
	}

	constexpr bool NeedsFill() const
	{
		return this->_second_row.IsEmpty();
	}

	const bool Execute()
	{
		Message("TODO!");
		return true;
	}

	constexpr void Fill(const PixelRowContainer& pixel_row_container)
	{
		if (this->IsEmpty())
		{
			this->_first_row = pixel_row_container;
		}
		else
		{
			this->_second_row = pixel_row_container;
		}
	}

private:
	PixelRowContainer _first_row{};
	PixelRowContainer _second_row{};
};
}

#endif // __LR35902_PIXEL_FIFO_H__