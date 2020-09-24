/**
 * @file		LCDRender.h
 * @author		Noam Rodrik
 * @description LR35902 pixel lcd render
 */
#ifndef __LR35902_LCD_RENDER_H__
#define __LR35902_LCD_RENDER_H__

#include <Core/GPU/Mechanics/PixelFIFO.h>
#include <Core/GPU/Mechanics/Fetcher.h>
#include <Core/GPU/Registers/LY.h>
#include <API/Definitions.h>
#include <Tools/Tools.h>

namespace Core
{
class LCDRender
{
public:
	constexpr LCDRender() = default;
	~LCDRender() = default;

private:
	enum class State
	{
		OAM_SEARCH = 0x00,
		PIXEL_RENDER = 0x01,
		H_BLANK = 0x02,
		V_BLANK = 0x03
	};

public:
	/**
	 * Executes next batch of instructions.
	 */
	constexpr bool Execute(std::size_t clocks)
	{
		this->_clocks += clocks;

		switch (this->_state)
		{
			case (State::OAM_SEARCH):
			{
				return this->OAMSearch();
			}

			case (State::PIXEL_RENDER):
			{
				return this->PixelRender();
			}

			case (State::H_BLANK):
			{
				return this->HBlank();
			}

			case (State::V_BLANK):
			{
				return this->VBlank();
			}

			default:
			{
				LOG("Got into an impossible state in the lcd renderer!");
				return false;
			}
		}
	}

private:
	constexpr bool OAMSearch()
	{
		RET_FALSE_IF_FAIL(this->NeedsOAMSearch(), "Running OAM search when not needed!");

		constexpr std::size_t OAM_SEARCH_AMOUNT_OF_CYCLES{20};
		if (this->_clocks >= OAM_SEARCH_AMOUNT_OF_CYCLES)
		{
			this->_clocks -= OAM_SEARCH_AMOUNT_OF_CYCLES;
			this->_state = State::PIXEL_RENDER;
		}

		return true;
	}

	constexpr bool HBlank()
	{
		return true;
	}

	constexpr bool VBlank()
	{
		return true;
	}

	constexpr bool PixelRender()
	{
		constexpr std::size_t PIXEL_FETCH_OPERATION_CLOCKS{2};
		while (this->_clocks >= PIXEL_FETCH_OPERATION_CLOCKS)
		{
			Message("TODO! Check for HBlank and VBLANK!");
			if (!this->_fifo.IsEmpty())
			{
				// FIFO executes once per clock. This passed 2 clocks so twice.
				RET_FALSE_IF_FAIL(this->_fifo.Execute(), "Failed executing the FIFO #1!");
				RET_FALSE_IF_FAIL(this->_fifo.Execute(), "Failed executing the FIFO #2!");
			}

			RET_FALSE_IF_FAIL(this->_fetcher.Execute(), "Failed executing the fetcher!");

			this->_clocks -= PIXEL_FETCH_OPERATION_CLOCKS;
		}

		// If there's still a clock remaining, use it on FIFO.
		if (this->_clocks == 1)
		{
			RET_FALSE_IF_FAIL(this->_fifo.Execute(), "Failed executing the FIFO #2!");
			this->_clocks = 0;
		}

		return true;
	}

private:
	constexpr const auto FetchPixel()
	{
		return this->_fifo.FetchNextPixel();
	}

	void UpdateLYRegister() const
	{
		LY ly{this->_x};
	}

	constexpr bool NeedsOAMSearch() const
	{
		return this->_x == 0;
	}

public:
	PixelFIFO   _fifo{};
	Fetcher     _fetcher{this->_fifo};
	State		_state{State::OAM_SEARCH};
	API::data_t _x{0x00};
	API::data_t _y{0x00};
	std::size_t _clocks{0};
};
}

#endif // __LR35902_LCD_RENDER_H__