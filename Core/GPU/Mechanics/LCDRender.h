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
#include <Core/GPU/Registers/LYC.h>
#include <Core/GPU/Registers/LY.h>
#include <API/Definitions.h>
#include <Tools/Tools.h>

namespace Core
{
class LCDRender
{
public:
	LCDRender(API::IMemoryDeviceAccess& memory, IPPU& ppu) :
		_memory{memory}, _ppu{ppu} {}
	~LCDRender() = default;

private:
	enum class State
	{
		H_BLANK = 0x00,
		V_BLANK = 0x01,
		OAM_SEARCH = 0x02,
		PIXEL_RENDER = 0x03
	};

public:
	/**
	 * Executes next batch of instructions.
	 */
	bool Execute(std::size_t clocks)
	{
		this->_clocks += clocks;

		while (clocks != 0)
		{
			switch (this->_state)
			{
				case (State::OAM_SEARCH):
				{
					RET_FALSE_IF_FAIL(this->OAMSearch(clocks), "Failed OAM search");
					break;
				}

				case (State::PIXEL_RENDER):
				{
					RET_FALSE_IF_FAIL(this->PixelRender(clocks), "Failed pixel render");
					break;
				}

				case (State::H_BLANK):
				{
					RET_FALSE_IF_FAIL(this->HBlank(clocks), "Failed HBlank");
					break;
				}

				case (State::V_BLANK):
				{
					RET_FALSE_IF_FAIL(this->VBlank(clocks), "Failed VBlank");
					break;
				}

				default:
				{
					MAIN_LOG("Got into an impossible state in the lcd renderer!");
					return false;
				}
			}
		}

		return true;
	}

	const auto& GetScreen() const
	{
		return this->_fifo.GetScreen();
	}

	bool ResetLCD()
	{
		// Reset LY
		LY ly{0x00};
		return this->InterruptLCDModeChange(State::H_BLANK);
	}

private:
	const bool OAMSearch(std::size_t& io_clocks_left)
	{
		// If we just initialized, interrupt an OAM search.
		if (!this->_initialized)
		{
			this->_initialized = true;
			this->_fifo.ResetNewFrame();
			this->_fetcher.ResetOffset();
			RET_FALSE_IF_FAIL(this->InterruptLCDModeChange(State::OAM_SEARCH),
							  "Failed changing LCDC_Status or interrupting");
		}

		io_clocks_left = 0;
		if (this->_clocks >= OAM_SEARCH_MAXIMUM_CYCLES)
		{
			this->_clocks -= OAM_SEARCH_MAXIMUM_CYCLES;
			io_clocks_left = this->_clocks;
			RET_FALSE_IF_FAIL(this->InterruptLCDModeChange(State::PIXEL_RENDER),
							  "Failed changing LCDC_Status or interrupting");
		}

		return true;
	}

	const bool HBlank(std::size_t& io_clocks_left)
	{		
		// Save this to fill the executed clocks from pixel render mode.
		this->_executed_clocks += this->_clocks;

		io_clocks_left = 0;
		if (this->_executed_clocks >= HBLANK_CLOCKS)
		{
			// If we need VBlank (starts from 0 so we need -1)
			if (this->NeedsVBlank())
			{
				// We need to go back to the beginning.
				RET_FALSE_IF_FAIL(this->InterruptLCDModeChange(State::V_BLANK), "Failed changing LCDC_Status or interrupting");

			}
			else
			{
				// Set if not in vblank
				this->_fifo.SetY(this->_fifo.GetY() + 1);
				this->_fetcher.NextRowOffset();

				// We need to return to OAM search.
				RET_FALSE_IF_FAIL(this->InterruptLCDModeChange(State::OAM_SEARCH), "Failed changing LCDC_Status or interrupting");

			}
			
			this->_clocks = this->_executed_clocks - HBLANK_CLOCKS;
			this->_executed_clocks = 0;
			io_clocks_left = this->_clocks;
		}

		return true;
	}

	bool VBlank(std::size_t& io_clocks_left)
	{
		io_clocks_left = 0;
		if (this->_clocks >= VBLANK_CLOCKS)
		{
			this->_lines += 1;
			this->_fifo.SetY(this->_fifo.GetY() + 1);
			this->_clocks -= VBLANK_CLOCKS;
			io_clocks_left = this->_clocks;

			if (this->_lines == VBLANK_LINES_LEN)
			{
				// Going back to the beginning.
				// FIRST FIFO and then FETCHER!
				this->_lines = 0;
				this->_fifo.ResetNewFrame();
				this->_fetcher.ResetOffset();

				RET_FALSE_IF_FAIL(this->InterruptLCDModeChange(State::OAM_SEARCH), "Failed changing LCDC_Status or interrupting");
			}
		}

		return true;
	}

	bool PixelRender(std::size_t& io_clocks_left)
	{
		// Save this to fill the executed clocks in HBlank mode.
		this->_executed_clocks += this->_clocks;

		io_clocks_left = 0;
		while (this->_clocks >= FIFO_PIXEL_CLOCKS)
		{
			// If we need an HBlank, we've gotten to the end.
			if (this->NeedsHBlank())
			{
				this->_fifo.ResetNewLine();
				this->_fetcher.Clear();

				io_clocks_left = this->_clocks;

				RET_FALSE_IF_FAIL(this->InterruptLCDModeChange(State::H_BLANK), "Failed changing LCDC_Status or interrupting");

				return true;
			}

			RET_FALSE_IF_FAIL(this->InnerExecute(), "Failed executing fifo and fetcher");
		}

		return true;
	}

	/**
	 * All combinations of possibilities of running the fetcher and fifo.
	 */
	constexpr bool InnerExecute()
	{
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
	const bool NeedsVBlank() const
	{
		// -1 since we start counting from 0
		return (this->_fifo.GetY() - this->_fifo.scy) >= SCREEN_HEIGHT_PIXELS - 1;
	}

	const bool NeedsHBlank() const
	{
		// -1 since we start counting from 0
		return (this->_fifo.GetX() - this->_fifo.scx) >= SCREEN_WIDTH_PIXELS - 1;
	}

public:
	static constexpr std::size_t OAM_SEARCH_MAXIMUM_CYCLES{80};
	static constexpr std::size_t PIXEL_RENDER_MAXIMUM_CYCLES{172};
	static constexpr std::size_t HBLANK_CLOCK_MAXIMUM_CYCLES{204};
	static constexpr std::size_t VBLANK_LINES_LEN{10};
	static constexpr std::size_t FIFO_PIXEL_CLOCKS{4};
	static constexpr std::size_t FETCHER_OPERATION_CLOCKS{8};
	static constexpr std::size_t HBLANK_CLOCKS{PIXEL_RENDER_MAXIMUM_CYCLES + HBLANK_CLOCK_MAXIMUM_CYCLES};
	static constexpr std::size_t VBLANK_CLOCKS{OAM_SEARCH_MAXIMUM_CYCLES + PIXEL_RENDER_MAXIMUM_CYCLES + HBLANK_CLOCK_MAXIMUM_CYCLES};

private:
	const bool InterruptLCDModeChange(State new_state)
	{
		RET_FALSE_IF_FAIL(static_cast<API::data_t>(new_state) <= 0x03, "Invalid new state");

		auto lcdc_status_register{LCDC_Status{}};
		auto lcdc_status{static_cast<LCDC_Status::Status>(lcdc_status_register)};
		RET_FALSE_IF_FAIL(lcdc_status.Validate(), "Failed validating LCDC status register");

		lcdc_status.lcd_enable = static_cast<API::data_t>(new_state);

		bool trigger_interrupt{false};

		// Changing modes in respect to new_state.
		switch (new_state)
		{
			case (State::H_BLANK):
			{
				if (lcdc_status.mode_0 == lcdc_status.MODE_SELECTION)
				{
					trigger_interrupt = true;
				}

				break;
			}

			case (State::V_BLANK):
			{
				if (lcdc_status.mode_1 == lcdc_status.MODE_SELECTION)
				{
					trigger_interrupt = true;
				}

				break;
			}

			case (State::OAM_SEARCH):
			{
				if (lcdc_status.mode_2 == lcdc_status.MODE_SELECTION)
				{
					trigger_interrupt = true;
				}

				break;
			}

			case (State::PIXEL_RENDER):
			{
				lcdc_status.coincidence_flag = lcdc_status.LYC_NOT_EQUAL_LCDC;
				if (this->_fifo.GetY() == static_cast<data_t>(LYC{}))
				{
					lcdc_status.coincidence_flag = lcdc_status.LYC_EQUAL_LCDC;

					if (lcdc_status.mode_lyc == lcdc_status.MODE_SELECTION)
					{
						trigger_interrupt = true;
					}
				}

				break;
			}
		}

		lcdc_status_register = lcdc_status;

		if (trigger_interrupt)
		{
			InterruptHandler::IRQ(EInterrupts::LCDC);
		}

		this->_state = new_state;

		return true;
	}

public:
	API::IMemoryDeviceAccess& _memory;
	IPPU&                     _ppu;
	PixelFIFO				  _fifo{_ppu};
	Fetcher					  _fetcher{this->_fifo, _ppu};
	State					  _state{State::OAM_SEARCH};
	std::size_t				  _clocks{0x00};
	std::size_t               _executed_clocks{0x00};
	bool                      _initialized{false};
	std::size_t               _lines{0x00};
};
}

#endif // __LR35902_LCD_RENDER_H__