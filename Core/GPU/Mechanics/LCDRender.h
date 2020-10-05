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
		this->_fifo.WhiteScreen();

		this->Reset();

		this->_fifo.SetY(this->_fifo.GetSCY());

		this->ChangeState(PPUState::OAM_SEARCH);
	}

	void Start()
	{
		this->_fifo.WhiteScreen();
	}

private:
	const bool OAMSearch()
	{
		if (this->_clocks >= OAM_SEARCH_MAXIMUM_CYCLES)
		{
			this->_clocks -= OAM_SEARCH_MAXIMUM_CYCLES;
			this->ChangeState(PPUState::PIXEL_RENDER);

			return true;
		}

		return false;
	}

	const bool HBlank()
	{		
		if (this->_clocks >= HBLANK_CLOCK_MINIMUM_CYCLES)
		{
			// If we need VBlank.
			if (this->_fifo.YPassedThreshold())
			{
				InterruptHandler::IRQ(EInterrupts::VBLANK);

				// We need to go back to the beginning.
				this->ChangeState(PPUState::V_BLANK);
			}
			else
			{
				// We need to return to OAM search.
				this->ChangeState(PPUState::OAM_SEARCH);

				// Increment Y value for FIFO
				this->Reset();
				this->_fifo.IncrementY();

				// Check LYC
				this->CompareLYC();
			}

			this->_clocks -= HBLANK_CLOCK_MINIMUM_CYCLES;

			return true;
		}

		return false;
	}

	bool VBlank()
	{
		if (this->_clocks >= VBLANK_CLOCKS)
		{
			this->_fifo.IncrementY();
			this->CompareLYC();
			this->_clocks -= VBLANK_CLOCKS;

			if (static_cast<API::data_t>(LY{}) == VBLANK_LY_END)
			{
				// Going back to the beginning.
				this->Reset();
				this->ChangeState(PPUState::OAM_SEARCH);
			}

			return true;
		}

		return false;
	}

	bool PixelRender()
	{
		if (this->_clocks >= PIXEL_RENDER_MAXIMUM_CYCLES)
		{
			std::size_t clocks{PIXEL_RENDER_MAXIMUM_CYCLES};

			while (!this->_fifo.XPassedThreshold())
			{
				SANITY(this->InnerExecute(clocks), "Failed executing fifo and fetcher");
			}

			this->_clocks -= PIXEL_RENDER_MAXIMUM_CYCLES;
			this->ChangeState(PPUState::H_BLANK);
			return true;
		}

		return false;
	}

	/**
	 * All combinations of possibilities of running the fetcher and fifo.
	 */
	const bool InnerExecute(std::size_t& io_clocks)
	{
		if (io_clocks == FIFO_PIXEL_CLOCKS)
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
		}
		else if (this->_fifo.NeedsFill())
		{
			RET_FALSE_IF_FAIL(this->_fetcher.Execute(FETCHER_OPERATION_CLOCKS), "Failed executing the fetcher!");
			RET_FALSE_IF_FAIL(this->_fifo.Execute(), "Failed executing the FIFO #1!");
			RET_FALSE_IF_FAIL(this->_fifo.Execute(), "Failed executing the FIFO #2!");

			io_clocks -= FETCHER_OPERATION_CLOCKS;
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

			io_clocks -= FETCHER_OPERATION_CLOCKS;
		}

		return true;
	}

private:
	void ChangeState(const PPUState new_state)
	{
		SANITY(static_cast<API::data_t>(new_state) <= 0x03, "Invalid new state");

		API::data_t new_lcdc_status{LCDC_Status{}};
		new_lcdc_status &= 0xFC;
		new_lcdc_status |= static_cast<API::data_t>(new_state);
		SANITY(this->_ppu.GetProcessor().GetMemory().WriteDirectly(LCDC_Status::LCDC_ADDRESS, new_lcdc_status), "Failed changing lcdc status");

		LCDC_Status lcdc_status{};
		bool interrupt_state{false};
		switch (new_state)
		{
			case PPUState::H_BLANK:
			{
				interrupt_state = (static_cast<LCDC_Status::Status>(lcdc_status).mode_0 == LCDC_Status::Status::MODE_SELECTION);
				break;
			}

			case PPUState::V_BLANK:
			{
				interrupt_state = (static_cast<LCDC_Status::Status>(lcdc_status).mode_1 == LCDC_Status::Status::MODE_SELECTION);
				break;
			}

			case PPUState::OAM_SEARCH:
			{
				interrupt_state = (static_cast<LCDC_Status::Status>(lcdc_status).mode_2 == LCDC_Status::Status::MODE_SELECTION);
				break;
			}
		}

		if (interrupt_state)
		{
			InterruptHandler::IRQ(EInterrupts::LCDC);
		}

		this->_state = new_state;
	}

	void CompareLYC() const
	{
		API::data_t lcdc_status{LCDC_Status{}};
		Message("Not sure if + 1 or without!?");
		Tools::MutateBitByCondition(static_cast<data_t>(LY{}) == static_cast<data_t>(LYC{}), lcdc_status, 2);

		if (static_cast<LCDC_Status::Status>(lcdc_status).mode_lyc == LCDC_Status::Status::MODE_SELECTION &&
			static_cast<LCDC_Status::Status>(lcdc_status).coincidence_flag == LCDC_Status::Status::LYC_EQUAL_LCDC)
		{
			InterruptHandler::IRQ(EInterrupts::LCDC);
		}

		SANITY(this->_ppu.GetProcessor().GetMemory().WriteDirectly(LCDC_Status::LCDC_ADDRESS, lcdc_status),
			   "Failed writing directly to status");
	}

	void Reset()
	{
		// First FIFO and then FETCHER
		this->_fifo.Reset();
		this->_fetcher.Reset();
	}

public:
	static constexpr std::size_t OAM_SEARCH_MAXIMUM_CYCLES{80};
	static constexpr std::size_t PIXEL_RENDER_MAXIMUM_CYCLES{289};
	static constexpr std::size_t HBLANK_CLOCK_MINIMUM_CYCLES{87};
	static_assert(HBLANK_CLOCK_MINIMUM_CYCLES + PIXEL_RENDER_MAXIMUM_CYCLES + OAM_SEARCH_MAXIMUM_CYCLES == 456,
		         "Bad clock amount");
	static constexpr std::size_t VBLANK_LY_END{0x00};
	static constexpr std::size_t FIFO_PIXEL_CLOCKS{4};
	static constexpr std::size_t FETCHER_OPERATION_CLOCKS{8};
	static constexpr std::size_t HBLANK_CLOCKS{PIXEL_RENDER_MAXIMUM_CYCLES + HBLANK_CLOCK_MINIMUM_CYCLES};
	static constexpr std::size_t VBLANK_CLOCKS{OAM_SEARCH_MAXIMUM_CYCLES + PIXEL_RENDER_MAXIMUM_CYCLES + HBLANK_CLOCK_MINIMUM_CYCLES};

public:
	API::IMemoryDeviceAccess& _memory;
	IPPU&                     _ppu;
	PixelFIFO				  _fifo{this->_ppu};
	Fetcher					  _fetcher{this->_fifo, this->_ppu};
	PPUState				  _state{PPUState::OAM_SEARCH};
	std::size_t				  _clocks{0x00};
};
} // Core

#endif // __LR35902_LCD_RENDER_H__