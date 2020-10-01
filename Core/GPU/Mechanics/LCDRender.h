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

public:
	/**
	 * Executes next batch of instructions.
	 */
	bool Execute(std::size_t clocks)
	{
		this->_clocks += clocks;

		while (this->_clocks != 0)
		{
			switch (this->_state)
			{
				case (PPUState::OAM_SEARCH):
				{
					if (!this->OAMSearch())
					{
						return true;
					}

					break;
				}

				case (PPUState::PIXEL_RENDER):
				{
					if (!this->PixelRender())
					{
						return true;
					}

					break;
				}

				case (PPUState::H_BLANK):
				{
					if (!this->HBlank())
					{
						return true;
					}

					break;
				}

				case (PPUState::V_BLANK):
				{
					if (!this->VBlank())
					{
						return true;
					}

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

	void ResetLCD()
	{
		this->_fifo.BlackScreen();

		this->_fifo.ResetNewLine();
		this->_fifo.ResetNewFrame();
		this->_fetcher.Reset();
		this->_fetcher.ResetNewFrame();

		this->ChangeState(PPUState::H_BLANK);
	}

	void Start()
	{
		this->_fifo.WhiteScreen();
	}

private:
	const bool OAMSearch()
	{
		static bool initialized{false};

		if (!initialized)
		{
			initialized = true;
			this->_fifo.ResetNewFrame();
			this->_fetcher.ResetNewFrame();
		}

		if (this->_clocks >= OAM_SEARCH_MAXIMUM_CYCLES)
		{
			this->_clocks -= OAM_SEARCH_MAXIMUM_CYCLES;

			this->_fifo.ResetNewLine();
			this->_fetcher.Reset();

			this->ChangeState(PPUState::PIXEL_RENDER);

			return true;
		}

		return false;
	}

	const bool HBlank()
	{		
		// Save this to fill the executed clocks from pixel render mode.
		this->_executed_clocks += this->_clocks;

		if (this->_executed_clocks >= HBLANK_CLOCKS)
		{
			this->_fifo.ResetNewLine();

			// If we need VBlank.
			if (this->NeedsVBlank())
			{
				// We need to go back to the beginning.
				this->ChangeState(PPUState::V_BLANK);
			}
			else
			{
				static IORAM* io_ram_memory_ptr{static_cast<IORAM*>(this->_ppu.GetProcessor().GetMemory().GetDeviceAtAddress(LCDC_Status::LCDC_ADDRESS))};
				Tools::MutateBitByCondition((LY{} + 1) == static_cast<data_t>(LYC{}),
										    io_ram_memory_ptr->GetMemoryPointer()[LCDC_Status::LCDC_ADDRESS - IORAM::START_ADDRESS],
											2);

				// Set if not in vblank
				// Fetcher must be after fifo.
				this->_fifo.IncrementY();
				this->_fetcher.NextRowOffset();

				// We need to return to OAM search.
				this->ChangeState(PPUState::OAM_SEARCH);
			}

			this->_fetcher.Reset();
			this->_clocks = this->_executed_clocks - HBLANK_CLOCKS;
			this->_executed_clocks = 0;

			return true;
		}

		return false;
	}

	bool VBlank()
	{
		if (this->_clocks >= VBLANK_CLOCKS)
		{
			this->_lines += 1;
			this->_fifo.ResetNewLine();
			this->_fifo.IncrementY();
			this->_clocks -= VBLANK_CLOCKS;

			if (this->_lines == VBLANK_LINES_LEN)
			{
				// Going back to the beginning.
				// FIRST FIFO and then FETCHER!
				this->_lines = 0;

				this->_fifo.ResetNewFrame();
				this->_fetcher.ResetNewFrame();

				this->ChangeState(PPUState::OAM_SEARCH);
			}

			return true;
		}

		return false;
	}

	bool PixelRender()
	{
		// Save this to fill the executed clocks in HBlank mode.
		this->_executed_clocks += this->_clocks;

		while (this->_clocks >= FIFO_PIXEL_CLOCKS)
		{
			// If we need an HBlank, we've gotten to the end.
			if (this->NeedsHBlank())
			{
				this->ChangeState(PPUState::H_BLANK);
				return true;
			}

			SANITY(this->InnerExecute(), "Failed executing fifo and fetcher");
		}

		return false;
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
		return this->_fifo.YPassedThreshold();
	}

	const bool NeedsHBlank() const
	{
		return this->_fifo.XPassedThreshold();
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
	const void ChangeState(PPUState new_state)
	{
		SANITY(static_cast<API::data_t>(new_state) <= 0x03, "Invalid new state");

		static IORAM* io_ram_memory_ptr{static_cast<IORAM*>(this->_ppu.GetProcessor().GetMemory().GetDeviceAtAddress(LCDC_Status::LCDC_ADDRESS))};
		io_ram_memory_ptr->GetMemoryPointer()[LCDC_Status::LCDC_ADDRESS - IORAM::START_ADDRESS] &= 0xFC;
		io_ram_memory_ptr->GetMemoryPointer()[LCDC_Status::LCDC_ADDRESS - IORAM::START_ADDRESS] |= static_cast<API::data_t>(new_state);

		this->_state = new_state;
	}

public:
	API::IMemoryDeviceAccess& _memory;
	IPPU&                     _ppu;
	PixelFIFO				  _fifo{this->_ppu};
	Fetcher					  _fetcher{this->_fifo, this->_ppu};
	PPUState				  _state{PPUState::OAM_SEARCH};
	std::size_t				  _clocks{0x00};
	std::size_t               _executed_clocks{0x00};
	std::size_t               _lines{0x00};
};
}

#endif // __LR35902_LCD_RENDER_H__