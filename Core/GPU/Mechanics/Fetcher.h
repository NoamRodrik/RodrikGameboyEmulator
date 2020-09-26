/**
 * @file		Fetcher.h
 * @author		Noam Rodrik
 * @description LR35902 fetcher object for pixel rows
 */
#ifndef __LR35902_FETCHER_H__
#define __LR35902_FETCHER_H__

#include <Core/GPU/Entities/PixelRowContainer.h>
#include <Core/GPU/Registers/LCDC_Control.h>
#include <Core/GPU/Mechanics/PixelFIFO.h>
#include <Core/Bus/Devices/VideoRAM.h>
#include <Core/GPU/Registers/SCY.h>
#include <API/Definitions.h>
#include <Tools/Tools.h>

namespace Core
{
/**
 * The fetcher contains 3 important stages (if not working with sprites!):
 * 
 * First it reads the tile # from the vram. (Takes 2 clocks)
 * Second, it reads the first part of the data for the row. (Takes 2 clocks)
 * Finally, it reads the second part of the data for the row. (Takes 2 clocks)
 * 
 * Once it finishes these steps, it waits until the PixelFIFO has exactly 1 container empty and
 * fills it with it's own (Takes 2 clocks).
 * 
 * This process is looped over and over again.
 */
class Fetcher
{
public:
	constexpr Fetcher(PixelFIFO& fifo, IPPU& ppu) : _fifo{fifo}, _ppu{ppu} {}
	~Fetcher() = default;

private:
	enum class State
	{
		FETCH_TILE = 0x00,
		READ_DATA_0 = 0x01,
		READ_DATA_1 = 0x02,
		WAIT_FOR_FIFO = 0x03
	};

public:
	void ResetOffset()
	{
		// Reset it to the first line of the SCY
		this->_tile_offset = (this->_fifo.scy - (this->_fifo.scy % PixelRow::PIXEL_COUNT)) * 4;
	}

	void Clear()
	{
		this->_pixel_row_container.Clear();
		this->_state = State::FETCH_TILE;
	}

	const void NextRowOffset()
	{
		// If we completed a row, we jump to the next row offset, otherwise we go back.
		if (this->_fifo.GetY() % 8 == 0 && this->_fifo.GetY() != 0)
		{
			// Complete the tile offset until the end of the row.
			this->_tile_offset += TILES_IN_ROW - (this->_tile_offset % TILES_IN_ROW);
		}
		else
		{
			// Go back.
			this->_tile_offset -= (this->_tile_offset % TILES_IN_ROW);
		}
	}

	bool Execute(std::size_t clocks)
	{
		this->_clocks += clocks;
		switch (this->_state)
		{
			case (State::FETCH_TILE):
			{
				auto lcdc_control_register{LCDC_Control{}};
				auto lcdc_control{static_cast<LCDC_Control::Control>(lcdc_control_register)};
				RET_FALSE_IF_FAIL(lcdc_control.Validate(), "Invalid lcdc control");

				if (lcdc_control.background_enable == lcdc_control.BACKGROUND_ON)
				{
					return this->FetchBackgroundTile();
				}

				return true;
			}

			case (State::READ_DATA_0):
			{
				return this->ReadDataUpper();
			}

			case (State::READ_DATA_1):
			{
				return this->ReadDataLower();
			}

			case (State::WAIT_FOR_FIFO):
			{
				return this->WaitForFIFO();
			}

			default:
			{
				MAIN_LOG("Got into an impossible state in the fetcher!");
				return false;
			}
		}
	}

private:
	/**
	 * Fetch tile state -> Read data 0 state
	 */
	bool FetchBackgroundTile()
	{
		if (this->_clocks >= FETCH_TILE_CLOCKS)
		{
			// If X isn't updated with SCX, update it.
			if (this->_fifo.GetX() < this->_fifo.scx)
			{
				this->_fifo.SetX(this->_fifo.scx);
				this->_tile_offset = this->_fifo.scx % PixelRow::PIXEL_COUNT;
			}

			auto lcdc_register{LCDC_Control{}};
			auto lcdc_control{static_cast<LCDC_Control::Control>(lcdc_register)};
			RET_FALSE_IF_FAIL(lcdc_control.Validate(), "Failed validating lcdc control");
			static auto* vram_memory_ptr{static_cast<VideoRAM*>(this->_ppu.GetProcessor().GetMemory().GetDeviceAtAddress(VideoRAM::START_ADDRESS))->GetMemoryPointer()};
			this->_tile_index = vram_memory_ptr[lcdc_control.GetBackgroundMapStart() + this->_tile_offset - VideoRAM::START_ADDRESS];
			this->_tile_offset += 1;
			this->_clocks -= FETCH_TILE_CLOCKS;
			this->_state = State::READ_DATA_0;
		}

		return true;
	}

	/**
	 * Read data 0 state -> Read data 1 state
	 */
	bool ReadDataUpper()
	{
		if (this->_clocks >= READ_DATA_0_CLOCKS)
		{
			auto& [container, source] = this->GetUpperTileByte();
			this->_pixel_row_container.SetUpper(container);
			this->_clocks -= READ_DATA_0_CLOCKS;
			this->_state = State::READ_DATA_1;
		}

		return true;
	}

	/**
	 * Read data 1 state -> Wait for FIFO state
	 */
	bool ReadDataLower()
	{
		if (this->_clocks >= READ_DATA_1_CLOCKS)
		{
			auto& [container, source] = this->GetLowerTileByte();
			this->_pixel_row_container.SetLower(container);
			this->_pixel_row_container.Initialize(source);
			this->_clocks -= READ_DATA_1_CLOCKS;
			this->_state = State::WAIT_FOR_FIFO;
		}

		return true;
	}

	/**
	 * Wait for FIFO state -> Fetch tile state
	 */
	bool WaitForFIFO()
	{
		if (this->_clocks >= WAIT_FOR_FIFO_CLOCKS)
		{
			// Will anyways use up clocks
			this->_clocks -= WAIT_FOR_FIFO_CLOCKS;

			if (this->_fifo.NeedsFill())
			{
				this->_fifo.Fill(this->_pixel_row_container);
				this->_pixel_row_container.Clear();
				this->_state = State::FETCH_TILE;
			}
		}

		return true;
	}

private:
	std::pair<API::data_t, PixelSource> GetUpperTileByte()
	{
		return this->GetTileInformation(false);
	}

	std::pair<API::data_t, PixelSource> GetLowerTileByte()
	{
		return this->GetTileInformation(true);
	}

	std::pair<API::data_t, PixelSource> GetTileInformation(bool lower = false)
	{
		auto lcdc_register{LCDC_Control{}};
		auto lcdc_control{static_cast<LCDC_Control::Control>(lcdc_register)};

		// Fetching address of the tile.
		const int TILE_INDEX_OFFSET{(lcdc_control.IsSigned() && this->_tile_index > 127) ? -1 * this->_tile_index : this->_tile_index};

		// Setting pixel row's upper to the fetched tile byte.
		// Fetching the tile according to the Y axis * 2 (2 bytes for each row, thus in jumps of 2)
		static auto* vram_memory_ptr{static_cast<VideoRAM*>(this->_ppu.GetProcessor().GetMemory().GetDeviceAtAddress(VideoRAM::START_ADDRESS))->GetMemoryPointer()};
		constexpr API::address_t SIZE_OF_TILE{16};
		Message("TODO! Make this customizable");
		return {vram_memory_ptr[lcdc_control.GetTileSelectOffset() + SIZE_OF_TILE * TILE_INDEX_OFFSET +
							  2 * (this->_fifo.GetY() % PixelRow::PIXEL_COUNT) +
							  static_cast<API::data_t>(lower) - VideoRAM::START_ADDRESS], PixelSource::BGP};
	}

private:
	static constexpr std::size_t READ_DATA_0_CLOCKS{8};
	static constexpr std::size_t READ_DATA_1_CLOCKS{8};
	static constexpr std::size_t WAIT_FOR_FIFO_CLOCKS{8};
	static constexpr std::size_t FETCH_TILE_CLOCKS{8};
	static constexpr std::size_t TILES_IN_ROW{0x20};

private:
	State             _state{State::FETCH_TILE};
	IPPU&             _ppu;
	API::address_t    _tile_offset{0x00};
	API::data_t       _tile_index{0x00};
	PixelRowContainer _pixel_row_container{};
	PixelFIFO&        _fifo;
	std::size_t       _clocks{0x00};
};
}

#endif // __LR35902_FETCHER_H__