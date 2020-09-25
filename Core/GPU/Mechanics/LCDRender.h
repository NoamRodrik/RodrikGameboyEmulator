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
	const bool OAMSearch()
	{
		RET_FALSE_IF_FAIL(this->NeedsOAMSearch(), "Running OAM search when not needed!");

		// If we just initialized, interrupt an OAM search.
		if (!this->_initialized)
		{
			this->_initialized = true;
			this->_fifo.ResetOffset();
			this->_fetcher.ResetOffset();
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
			if (this->NeedsVBlank())
			{
				RET_FALSE_IF_FAIL(this->InterruptLCDModeChange(LCDC_Status::Status::DURING_V_BLANK), "Failed changing LCDC_Status or interrupting");

				// We need to go back to the beginning.
				this->_state = State::V_BLANK;
			}
			else
			{
				// Set if not in vblank
				this->_fifo.SetY(this->_fifo.GetY() + 1);

				RET_FALSE_IF_FAIL(this->InterruptLCDModeChange(LCDC_Status::Status::DURING_SEARCH_OAM_RAM), "Failed changing LCDC_Status or interrupting");

				// We need to return to OAM search.
				this->_state = State::OAM_SEARCH;
			}
			
			this->_clocks = this->_executed_clocks - HBLANK_CLOCKS;
			this->_executed_clocks = 0;
		}

		this->_clocks = 0;

		return true;
	}

	constexpr bool VBlank()
	{
		constexpr auto VBLANK_CLOCKS{OAM_SEARCH_MAXIMUM_CYCLES + PIXEL_RENDER_MAXIMUM_CYCLES + HBLANK_CLOCK_MAXIMUM_CYCLES};
		if (this->_clocks >= VBLANK_CLOCKS)
		{
			this->_lines += 1;
			this->_fifo.SetY(this->_fifo.GetY() + 1);
			this->_clocks -= VBLANK_CLOCKS;

			if (this->_lines == VBLANK_LINES_LEN)
			{
				// Going back to the beginning.
				// FIRST FIFO and then FETCHER!
				this->_lines = 0;
				this->_fifo.ResetOffset();
				this->_fetcher.ResetOffset();
				this->_state = State::OAM_SEARCH;
			}
		}

		return true;
	}

	constexpr bool PixelRender()
	{
		// Save this to fill the executed clocks in HBlank mode.
		this->_executed_clocks += this->_clocks;

		constexpr std::size_t FIFO_PIXEL_CLOCKS{4};
		while (this->_clocks >= FIFO_PIXEL_CLOCKS)
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

		this->_clocks = 0;

		return true;
	}

	/**
	 * All combinations of possibilities of running the fetcher and fifo.
	 */
	constexpr bool InnerExecute()
	{
		constexpr std::size_t FIFO_PIXEL_CLOCKS{4};
		constexpr std::size_t FETCHER_OPERATION_CLOCKS{8};
		if (this->_clocks == FIFO_PIXEL_CLOCKS)
		{
			if (this->_fifo.NeedsFill())
			{
				RET_FALSE_IF_FAIL(this->_fetcher.Execute(FIFO_PIXEL_CLOCKS), "Failed executing the fetcher!");
				RET_FALSE_IF_FAIL(this->_fifo.Execute(), "Failed executing the FIFO!");
			}
			else
			{
				RET_FALSE_IF_FAIL(this->_fifo.Execute(), "Failed executing the FIFO!");
				RET_FALSE_IF_FAIL(this->_fetcher.Execute(FIFO_PIXEL_CLOCKS), "Failed executing the fetcher!");
			}

			this->_clocks = 0;
		}
		else if (this->_fifo.NeedsFill())
		{
			RET_FALSE_IF_FAIL(this->_fetcher.Execute(FETCHER_OPERATION_CLOCKS), "Failed executing the fetcher!");
			RET_FALSE_IF_FAIL(this->_fifo.Execute(), "Failed executing the FIFO #1!");
			RET_FALSE_IF_FAIL(this->_fifo.Execute(), "Failed executing the FIFO #2!");

			this->_clocks -= FETCHER_OPERATION_CLOCKS;
		}
		else
		{
			RET_FALSE_IF_FAIL(this->_fifo.Execute(), "Failed executing the FIFO #1!");

			if (this->_fifo.NeedsFill())
			{
				RET_FALSE_IF_FAIL(this->_fetcher.Execute(FETCHER_OPERATION_CLOCKS), "Failed executing the fetcher!");
				RET_FALSE_IF_FAIL(this->_fifo.Execute(), "Failed executing the FIFO #2!");
			}
			else
			{
				RET_FALSE_IF_FAIL(this->_fifo.Execute(), "Failed executing the FIFO #2!");
				RET_FALSE_IF_FAIL(this->_fetcher.Execute(FETCHER_OPERATION_CLOCKS), "Failed executing the fetcher!");
			}

			this->_clocks -= FETCHER_OPERATION_CLOCKS;
		}

		return true;
	}

private:
	const bool NeedsOAMSearch() const
	{
		return this->_fifo.GetX() == 0x00;
	}

	const bool NeedsVBlank() const
	{
		// -1 since we start counting from 0
		return (this->_fifo.GetY() - this->_fifo.scy) >= SCREEN_HEIGHT_PIXELS - 1;
	}

	const bool NeedsHBlank() const
	{
		return (this->_fifo.GetX() - this->_fifo.scx) >= SCREEN_WIDTH_PIXELS - 1;
	}

public:
	static constexpr std::size_t OAM_SEARCH_MAXIMUM_CYCLES{80};
	static constexpr std::size_t PIXEL_RENDER_MAXIMUM_CYCLES{172};
	static constexpr std::size_t HBLANK_CLOCK_MAXIMUM_CYCLES{204};
	static constexpr std::size_t VBLANK_LINES_LEN{10};

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
	std::size_t               _lines{0x00};
};
}

#endif // __LR35902_LCD_RENDER_H__