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
	void ResetNewLine()
	{
		this->_lower_row.Clear();
		this->_upper_row.Clear();
		this->scx = SCX{};
		this->scy = SCY{};
		this->SetX(0x00);
	}

	void ResetNewFrame()
	{
		this->SetY(this->scy);
	}

	auto FetchNextPixel()
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

			return FETCHED_PALETTE_COLOR;
		}
		else
		{
			return this->_upper_row.GetNextPixel();
		}
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

		if (this->scx <= this->GetX())
		{
			const API::data_t DRAWN_X = this->GetX() - this->scx;
			const API::data_t DRAWN_Y = this->GetY() - this->scy;

			RET_FALSE_IF_FAIL(this->DrawPalette(DRAWN_X, DRAWN_Y, PIXEL),
							  "Failed drawing palette (%u, %u) for SCX %u and SCY %u, x %u y %u!",
			                    DRAWN_X, DRAWN_Y, this->scx, this->scy, this->GetX(), this->GetY());
			
		}

		this->SetX(this->GetX() + 1);

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

		// Pass pixels if necessary.
		for (std::size_t current_bit = 0; current_bit < this->_lower_row.EmptyBitsAmount(); ++current_bit)
		{
			this->_lower_row.SetLastPixel(this->_upper_row.GetNextPixel());
		}
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
		static auto* io_ram_memory_ptr{static_cast<IORAM*>(this->_ppu.GetProcessor().GetMemory().GetDeviceAtAddress(LY::LY_ADDRESS))->GetMemoryPointer()};
		io_ram_memory_ptr[LY::LY_ADDRESS - IORAM::START_ADDRESS] = y;
	}

	const API::data_t GetY() const
	{
		static auto* io_ram_memory_ptr{ static_cast<IORAM*>(this->_ppu.GetProcessor().GetMemory().GetDeviceAtAddress(LY::LY_ADDRESS))->GetMemoryPointer() };
		return io_ram_memory_ptr[LY::LY_ADDRESS - IORAM::START_ADDRESS];
	}

	const API::data_t GetX() const
	{
		return this->_x;
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

public:
	API::data_t scx{0x00};
	API::data_t scy{0x00};

private:
	API::data_t			      _x{0x00};
	IPPU&                     _ppu;
	PixelRowContainer		  _lower_row{};
	PixelRowContainer		  _upper_row{};
	std::array<std::array<olc::Pixel, SCREEN_WIDTH_PIXELS>, SCREEN_HEIGHT_PIXELS> _screen{};
};
}

#endif // __LR35902_PIXEL_FIFO_H__