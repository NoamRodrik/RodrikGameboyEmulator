/**
 * @file		PixelFIFO.h
 * @author		Noam Rodrik
 * @description LR35902 pixel FIFO header
 */
#ifndef __LR35902_PIXEL_FIFO_H__
#define __LR35902_PIXEL_FIFO_H__

#include <API/Memory/Device/IMemoryDeviceAccess.h>
#include <Core/GPU/Entities/PixelRowContainer.h>
#include <Core/GPU/Registers/SCY.h>
#include <Core/GPU/Registers/SCX.h>
#include <Core/GPU/Registers/LY.h>
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
	constexpr PixelFIFO(API::IMemoryDeviceAccess& memory, IScreenDrawer& screen_drawer) : _memory{memory}, _screen_drawer{screen_drawer} {}
	~PixelFIFO() = default;

public:
	void Clear()
	{
		this->_lower_row.Clear();
		this->_upper_row.Clear();
		this->scx = SCX{};
		this->SetX(0x00);
	}

	void ResetOffset()
	{
		this->scx = SCX{};
		this->scy = SCY{};
		this->SetX(0x00);
		this->SetY(this->scy);
	}

	constexpr auto FetchNextPixel()
	{
		SANITY(!this->IsEmpty(), "Both pixel containers in the FIFO are empty!");

		// Fetching from the first row if it isn't empty.
		if (!this->_lower_row.IsEmpty())
		{
			const auto FETCHED_PALETTE_COLOR{this->_lower_row.GetNextPixel()};
			
			// If the second row isn't empty, take the left bit from the second row
			// and apply it to the right bit of the first row.
			if (!this->_upper_row.IsEmpty())
			{
				this->_lower_row.SetLastPixel(this->_upper_row.GetNextPixel());
			}

			return FETCHED_PALETTE_COLOR;
		}
		else
		{
			return this->_upper_row.GetNextPixel();
		}
	}

	constexpr bool IsEmpty() const
	{
		return this->_lower_row.IsEmpty() && this->_upper_row.IsEmpty();
	}

	constexpr bool NeedsFill() const
	{
		return this->_upper_row.IsEmpty();
	}

	const bool Execute()
	{
		// If we need to fill the FIFO, we can't fetch pixels yet.
		// If we passed the screen, we don't need to draw.
		if (this->NeedsFill() || 
			(this->GetX() - this->scx) == SCREEN_WIDTH_PIXELS ||
			(this->GetY() - this->scy) == SCREEN_HEIGHT_PIXELS)
		{
			return true;
		}

		const auto PIXEL{this->FetchNextPixel()};

		if (this->scx <= this->GetX())
		{
			const API::data_t DRAWN_X = this->GetX() - this->scx;
			const API::data_t DRAWN_Y = this->GetY() - this->scy;

			RET_FALSE_IF_FAIL(this->_screen_drawer.DrawPalette(DRAWN_X, DRAWN_Y, PIXEL),
							  "Failed drawing palette (%u, %u) for SCX %u and SCY %u, x %u y %u!",
			                    DRAWN_X, DRAWN_Y, this->scx, this->scy, this->GetX(), this->GetY());
			
		}

		this->SetX(this->GetX() + 1);

		return true;
	}

	constexpr void Fill(const PixelRowContainer& pixel_row_container)
	{
		SANITY(!pixel_row_container.IsEmpty(), "Got an empty pixel row container");

		if (this->IsEmpty())
		{
			this->_lower_row = pixel_row_container;
		}
		else
		{
			this->_upper_row = pixel_row_container;
		}

		// Pass pixels if necessary.
		for (std::size_t current_bit = 0; current_bit < this->_lower_row.EmptyBitsAmount(); ++current_bit)
		{
			this->_lower_row.SetLastPixel(this->_upper_row.GetNextPixel());
		}
	}

	inline constexpr API::IMemoryDeviceAccess& GetMemory()
	{
		return this->_memory;
	}

	constexpr void SetX(const API::data_t x)
	{
		this->x = x;
	}

	void SetY(const API::data_t y)
	{
		this->_screen_drawer.SetYCoordinate(y);
	}

	const API::data_t GetY() const
	{
		return LY{};
	}

	constexpr const API::data_t GetX() const
	{
		return this->x;
	}
	
public:
	API::data_t scx{0x00};
	API::data_t scy{0x00};

private:
	API::data_t x{0x00};
	API::IMemoryDeviceAccess& _memory;
	IScreenDrawer&            _screen_drawer;
	PixelRowContainer		  _lower_row{};
	PixelRowContainer		  _upper_row{};
};
}

#endif // __LR35902_PIXEL_FIFO_H__