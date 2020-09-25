/**
 * @file		LCDRender.h
 * @author		Noam Rodrik
 * @description LR35902 pixel lcd render
 */
#ifndef __LR35902_LCD_RENDER_H__
#define __LR35902_LCD_RENDER_H__

#include <Core/GPU/Registers/LCDC_Status.h>
#include <Core/GPU/Mechanics/PixelFIFO.h>
#include <Core/GPU/Mechanics/Fetcher.h>
#include <Core/GPU/Registers/SCX.h>
#include <Core/GPU/Registers/SCY.h>
#include <Core/GPU/Registers/LY.h>
#include <API/Definitions.h>
#include <Tools/Tools.h>

namespace Core
{
class LCDRender
{
public:
	constexpr LCDRender(API::IMemoryDeviceAccess& memory, IScreenDrawer& screen_drawer) : _memory{memory}, _fifo{memory, screen_drawer} {}
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
				MAIN_LOG("Got into an impossible state in the lcd renderer!");
				return false;
			}
		}
	}

private:
	constexpr bool OAMSearch()
	{
		RET_FALSE_IF_FAIL(this->NeedsOAMSearch(), "Running OAM search when not needed!");

		// If we just initialized, interrupt an OAM search.
		if (!this->_initialized)
		{
			this->_initialized = true;
			this->_fifo.ResetOffset();
			RET_FALSE_IF_FAIL(this->InterruptLCDModeChange(LCDC_Status::Status::DURING_SEARCH_OAM_RAM),
							  "Failed changing LCDC_Status or interrupting");

		}

		if (this->_clocks >= OAM_SEARCH_MAXIMUM_CYCLES)
		{
			RET_FALSE_IF_FAIL(this->InterruptLCDModeChange(LCDC_Status::Status::DURING_DATA_TRANSFER_LCD),
							  "Failed changing LCDC_Status or interrupting");
			this->_clocks -= OAM_SEARCH_MAXIMUM_CYCLES;
			this->_state = State::PIXEL_RENDER;
		}

		return true;
	}

	constexpr bool HBlank()
	{
		// Save this to fill the executed clocks from pixel render mode.
		this->_executed_clocks += this->_clocks;

		constexpr auto HBLANK_CLOCKS{PIXEL_RENDER_MAXIMUM_CYCLES + HBLANK_CLOCK_MAXIMUM_CYCLES};

		if (this->_executed_clocks >= HBLANK_CLOCKS)
		{
			this->_fifo.SetY(this->_fifo.y + 1);

			if (this->NeedsVBlank())
			{
				RET_FALSE_IF_FAIL(this->InterruptLCDModeChange(LCDC_Status::Status::DURING_V_BLANK), "Failed changing LCDC_Status or interrupting");

				// We need to go back to the beginning.
				this->_state = State::V_BLANK;
			}
			else
			{
				RET_FALSE_IF_FAIL(this->InterruptLCDModeChange(LCDC_Status::Status::DURING_SEARCH_OAM_RAM), "Failed changing LCDC_Status or interrupting");

				// We need to return to OAM search.
				this->_state = State::OAM_SEARCH;
			}
			
			this->_executed_clocks = 0;
			this->_clocks = this->_executed_clocks - HBLANK_CLOCKS;
		}

		return true;
	}

	constexpr bool VBlank()
	{
		constexpr auto VBLANK_CLOCKS{OAM_SEARCH_MAXIMUM_CYCLES + PIXEL_RENDER_MAXIMUM_CYCLES + HBLANK_CLOCK_MAXIMUM_CYCLES};
		if (this->_clocks >= VBLANK_CLOCKS)
		{
			// Going back to the beginning.
			// FIRST FIFO and then FETCHER!
			this->_fifo.ResetOffset();
			this->_fetcher.ResetOffset();
			this->_clocks -= VBLANK_CLOCKS;
			this->_state = State::OAM_SEARCH;
		}

		return true;
	}

	constexpr bool PixelRender()
	{
		// Save this to fill the executed clocks in HBlank mode.
		this->_executed_clocks += this->_clocks;

		constexpr std::size_t PIXEL_FETCH_OPERATION_CLOCKS{2};
		while (this->_clocks >= PIXEL_FETCH_OPERATION_CLOCKS)
		{
			// If we need an HBlank, we've gotten to the end.
			if (this->NeedsHBlank())
			{
				this->_fifo.Clear();
				this->_fetcher.Clear();
				this->_fetcher.NextRowOffset();

				RET_FALSE_IF_FAIL(this->InterruptLCDModeChange(LCDC_Status::Status::DURING_H_BLANK), "Failed changing LCDC_Status or interrupting");
				this->_state = State::H_BLANK;

				return true;
			}

			RET_FALSE_IF_FAIL(this->InnerExecute(), "Failed executing fifo and fetcher");
		}

		// If there's still a clock remaining, use it on FIFO.
		while (this->_clocks > 0)
		{
			RET_FALSE_IF_FAIL(this->_fifo.Execute(), "Failed executing the FIFO!");
			this->_clocks -= 1;
		}

		return true;
	}

	/**
	 * All combinations of possibilities of running the fetcher and fifo.
	 */
	constexpr bool InnerExecute()
	{
		if (this->_fifo.NeedsFill())
		{
			RET_FALSE_IF_FAIL(this->_fetcher.Execute(this->_clocks), "Failed executing the fetcher!");
			RET_FALSE_IF_FAIL(this->_fifo.Execute(), "Failed executing the FIFO #1!");
			RET_FALSE_IF_FAIL(this->_fifo.Execute(), "Failed executing the FIFO #2!");
		}
		else
		{
			RET_FALSE_IF_FAIL(this->_fifo.Execute(), "Failed executing the FIFO #1!");

			if (this->_fifo.NeedsFill())
			{
				RET_FALSE_IF_FAIL(this->_fetcher.Execute(this->_clocks), "Failed executing the fetcher!");
				RET_FALSE_IF_FAIL(this->_fifo.Execute(), "Failed executing the FIFO #2!");
			}
			else
			{
				RET_FALSE_IF_FAIL(this->_fifo.Execute(), "Failed executing the FIFO #2!");
				RET_FALSE_IF_FAIL(this->_fetcher.Execute(this->_clocks), "Failed executing the fetcher!");
			}
		}

		return true;
	}

private:
	constexpr bool NeedsOAMSearch() const
	{
		return this->_fifo.x == 0;
	}

	const bool NeedsVBlank() const
	{
		return (this->_fifo.y - this->_fifo.scy) == SCREEN_HEIGHT_PIXELS;
	}

	const bool NeedsHBlank() const
	{
		return (this->_fifo.x - this->_fifo.scx) == SCREEN_WIDTH_PIXELS;
	}

public:
	static constexpr std::size_t OAM_SEARCH_MAXIMUM_CYCLES{80};
	static constexpr std::size_t PIXEL_RENDER_MAXIMUM_CYCLES{172};
	static constexpr std::size_t HBLANK_CLOCK_MAXIMUM_CYCLES{204};

private:
	constexpr bool InterruptLCDModeChange(API::data_t new_mode)
	{
		RET_FALSE_IF_FAIL(new_mode <= 0x03, "Invalid new mode");

		auto lcdc_status_register{LCDC_Status{}};
		auto lcdc_status{static_cast<LCDC_Status::Status>(lcdc_status_register)};
		RET_FALSE_IF_FAIL(lcdc_status.Validate(), "Failed validating LCDC status register");

		lcdc_status.lcd_enable = new_mode;
		lcdc_status_register = lcdc_status;
		InterruptHandler::IRQ(EInterrupts::LCDC);
		return true;
	}

public:
	API::IMemoryDeviceAccess& _memory;
	PixelFIFO				  _fifo;
	Fetcher					  _fetcher{this->_fifo};
	State					  _state{State::OAM_SEARCH};
	std::size_t				  _clocks{0x00};
	std::size_t               _executed_clocks{0x00};
	bool                      _initialized{false};
};
}

#endif // __LR35902_LCD_RENDER_H__