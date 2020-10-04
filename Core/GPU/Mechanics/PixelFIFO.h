/**
 * @file		PixelFIFO.h
 * @author		Noam Rodrik
 * @description LR35902 pixel FIFO header
 */
#ifndef __LR35902_PIXEL_FIFO_H__
#define __LR35902_PIXEL_FIFO_H__

#include <Contrib/PixelGameEngine/OLCPixelGameEngine.h>
#include <Core/GPU/Entities/PixelRowContainer.h>
#include <Core/GPU/Entities/PaletteMap.h>
#include <Core/Bus/Devices/IORAM.h>
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
	PixelFIFO(IPPU& ppu) : _ppu{ppu} {}
	~PixelFIFO() = default;

public:
	void Reset()
	{
		this->_lower_row.Clear();
		this->_upper_row.Clear();
		this->_scx = SCX{};
		this->SetX(this->GetSCX());
	}

	void IncrementY()
	{
		this->SetY(this->GetY() + 1);
	}

	std::pair<PixelSource, PaletteColor> FetchNextPixel()
	{
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

			return {this->_lower_row.GetSource(), FETCHED_PALETTE_COLOR};
		}

		return {PixelSource::BGP, PaletteColor::FIRST_PALETTE};
	}

	bool IsEmpty() const
	{
		return this->_lower_row.IsEmpty() && this->_upper_row.IsEmpty();
	}

	bool NeedsFill() const
	{
		return this->_upper_row.IsEmpty();
	}

	const bool Execute()
	{
		// If we need to fill the FIFO, we can't fetch pixels yet.
		// If we passed the screen, we don't need to draw.
		if (this->NeedsFill())
		{
			return true;
		}

		const auto PIXEL{this->FetchNextPixel()};
		if (!this->XPassedThreshold())
		{
			const API::data_t DRAWN_X{static_cast<API::data_t>(GetWrappedAroundDistance(this->GetX(), this->GetSCX()))};
			const API::data_t DRAWN_Y{static_cast<API::data_t>(LY{})};
			const LCDC_Control::Control lcdc_control{LCDC_Control{}};

			if (lcdc_control.background_enable == LCDC_Control::Control::BACKGROUND_ON && PIXEL.first == PixelSource::BGP ||
				lcdc_control.window_enable == LCDC_Control::Control::WINDOW_ON && PIXEL.first == PixelSource::WIN)
			{
				RET_FALSE_IF_FAIL(this->DrawPalette(DRAWN_X, DRAWN_Y, PIXEL.second),
					"Failed drawing palette (%u, %u) for SCX %u and SCY %u, x %u y %u!",
					DRAWN_X, DRAWN_Y, this->GetSCX(), this->GetSCY(), this->GetX(), this->GetY());
			}
			else
			{
				// If both bg and window are disabled, we draw the 0x00 palette.
				RET_FALSE_IF_FAIL(this->DrawPalette(DRAWN_X, DRAWN_Y, PaletteColor::FIRST_PALETTE),
					"Failed drawing palette (%u, %u) for SCX %u and SCY %u, x %u y %u!",
					DRAWN_X, DRAWN_Y, this->GetSCX(), this->GetSCY(), this->GetX(), this->GetY());
			}
		}

		this->SetX((this->GetX() + 1) % 0x100);

		return true;
	}

	void Fill(const PixelRowContainer& pixel_row_container)
	{
		if (this->IsEmpty())
		{
			this->_lower_row = pixel_row_container;
		}
		else
		{
			this->_upper_row = pixel_row_container;
		}

		SANITY(this->_lower_row.EmptyBitsAmount() == 0, "Serious bug occurred");
	}

	const auto& GetScreen() const
	{
		return this->_screen;
	}

	void SetX(const API::data_t x)
	{
		this->_x = x;
	}

	void SetY(const API::data_t y)
	{
		this->_y = y;
		static auto* io_ram_memory_ptr{static_cast<IORAM*>(this->_ppu.GetProcessor().GetMemory().GetDeviceAtAddress(LY::LY_ADDRESS))->GetMemoryPointer()};
		io_ram_memory_ptr[LY::LY_ADDRESS - IORAM::START_ADDRESS] = GetWrappedAroundDistance(this->_y, this->_scy) % 0x9A;

		if (LY{} == 0)
		{
			// Reset Y back to SCY
			this->_scy = SCY{};
			this->_y = this->GetSCY();
		}
	}

	const API::data_t GetY() const
	{
		return this->_y;
	}

	const API::data_t GetX() const
	{
		return this->_x;
	}

	void WhiteScreen()
	{
		for (std::size_t height = 0; height < SCREEN_HEIGHT_PIXELS; ++height)
		{
			for (std::size_t width = 0; width < SCREEN_WIDTH_PIXELS; ++width)
			{
				SANITY(this->DrawPixel(width, height, PixelColor::WHITE), "Failed drawing pixels");
			}
		}
	}

	const bool YPassedThreshold() const
	{
		return GetWrappedAroundDistance(this->GetY(), this->GetSCY()) >= SCREEN_HEIGHT_PIXELS - 1;
	}

	const bool XPassedThreshold() const
	{
		return GetWrappedAroundDistance(this->GetX(), this->GetSCX()) >= SCREEN_WIDTH_PIXELS;
	}

	const API::data_t GetSCY() const
	{
		return this->_scy;
	}

	const API::data_t GetSCX() const
	{
		return this->_scx;
	}

private:
	bool DrawPalette(int32_t x, int32_t y, PaletteColor color)
	{
		return DrawPixel(x, y, PaletteMap::ColorOf(color));
	}

	bool DrawPixel(int32_t x, int32_t y, PixelColor color)
	{
		switch (color)
		{
			case (PixelColor::WHITE):
			{
				this->_screen[y][x] = olc::Pixel{WHITE_PIXEL[0], WHITE_PIXEL[1], WHITE_PIXEL[2]};
				break;
			}

			case (PixelColor::BLACK):
			{
				this->_screen[y][x] = olc::Pixel{BLACK_PIXEL[0], BLACK_PIXEL[1], BLACK_PIXEL[2]};
				break;
			}

			case (PixelColor::LIGHT_GREY):
			{
				this->_screen[y][x] = olc::Pixel{LIGHT_GREY_PIXEL[0], LIGHT_GREY_PIXEL[1], LIGHT_GREY_PIXEL[2]};
				break;
			}

			case (PixelColor::DARK_GREY):
			{
				this->_screen[y][x] = olc::Pixel{DARK_GREY_PIXEL[0], DARK_GREY_PIXEL[1], DARK_GREY_PIXEL[2]};
				break;
			}

			default:
			{
				MAIN_LOG("Got an invalid pixel color: %u", static_cast<uint32_t>(color));
				return false;
			}
		}

		return true;
	}

private:
	std::size_t			      _x{0x00};
	std::size_t               _y{0x00};
	std::size_t               _scy{0x00};
	std::size_t               _scx{0x00};
	IPPU&					  _ppu;
	PixelRowContainer		  _lower_row{};
	PixelRowContainer		  _upper_row{};
	std::array<std::array<olc::Pixel, SCREEN_WIDTH_PIXELS>, SCREEN_HEIGHT_PIXELS> _screen{};
};
}

#endif // __LR35902_PIXEL_FIFO_H__