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
		this->SetX(0x00);
		this->scx = SCX{};
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
		SANITY(!this->_lower_row.IsEmpty() || !this->_upper_row.IsEmpty(),
			   "Both pixel containers in the FIFO are empty!");

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
		if (this->x < this->scx)
		{
			// Ignore the pixel.
			this->FetchNextPixel();
		}
		else
		{
			const API::data_t DRAWN_X = this->x - static_cast<API::data_t>(this->scx);
			const API::data_t DRAWN_Y = this->y - static_cast<API::data_t>(this->scy);

			RET_FALSE_IF_FAIL(this->_screen_drawer.DrawPalette(DRAWN_X, DRAWN_Y, this->FetchNextPixel()),
							  "Failed drawing palette (%u, %u) for SCX %u and SCY %u, x %u y %u!",
			                    DRAWN_X, DRAWN_Y, static_cast<API::data_t>(this->scx), static_cast<API::data_t>(this->scy),
				                this->x, this->y);
			
			this->SetX(this->x + 1);
		}

		return true;
	}

	constexpr void Fill(const PixelRowContainer& pixel_row_container)
	{
		if (this->IsEmpty())
		{
			this->_lower_row = pixel_row_container;
		}
		else
		{
			this->_upper_row = pixel_row_container;
		}
	}

	inline constexpr API::IMemoryDeviceAccess& GetMemory()
	{
		return this->_memory;
	}

	void SetX(const API::data_t x)
	{
		this->x = x;
		LY ly{x};
	}

	void SetY(const API::data_t y)
	{
		this->y = y;
	}

public:
	API::data_t x{0x00};
	API::data_t scx{0x00};
	API::data_t y{0x00};
	API::data_t scy{0x00};

private:
	API::IMemoryDeviceAccess& _memory;
	IScreenDrawer&            _screen_drawer;
	PixelRowContainer		  _lower_row{};
	PixelRowContainer		  _upper_row{};
};
}

#endif // __LR35902_PIXEL_FIFO_H__