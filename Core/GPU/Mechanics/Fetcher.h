/**
 * @file		Fetcher.h
 * @author		Noam Rodrik
 * @description LR35902 fetcher object for pixel rows
 */
#ifndef __LR35902_FETCHER_H__
#define __LR35902_FETCHER_H__

#include <Core/GPU/Entities/PixelRowContainer.h>
#include <Core/GPU/Registers/LCDC_Control.h>
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
	constexpr Fetcher(PixelFIFO& fifo) : _fifo{fifo} {}
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

	constexpr void Clear()
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

	constexpr bool Execute(std::size_t clocks)
	{
		this->_clocks += clocks;

		switch (this->_state)
		{
			case (State::FETCH_TILE):
			{
				return this->FetchTile();
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
	constexpr bool FetchTile()
	{
		if (this->_clocks >= FETCH_TILE_CLOCKS)
		{
			auto lcdc_register{LCDC_Control{}};
			auto lcdc_control{static_cast<LCDC_Control::Control>(lcdc_register)};
			RET_FALSE_IF_FAIL(lcdc_control.Validate(), "Failed validating lcdc control");
			RET_FALSE_IF_FAIL(this->_fifo.GetMemory().Read(lcdc_control.GetBackgroundMapStart() + this->_tile_offset, this->_tile_index),
							  "Failed reading memory for canvas");

			this->_tile_offset += 1;
			this->_clocks -= FETCH_TILE_CLOCKS;
			this->_state = State::READ_DATA_0;
		}

		return true;
	}

	/**
	 * Read data 0 state -> Read data 1 state
	 */
	constexpr bool ReadDataUpper()
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
	constexpr bool ReadDataLower()
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
	constexpr bool WaitForFIFO()
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
		SANITY(lcdc_control.Validate(), "Failed validating lcdc control");

		// Fetching address of the tile.
		constexpr API::address_t SIZE_OF_TILE{16};
		const int TILE_INDEX_OFFSET{(lcdc_control.IsSigned() && this->_tile_index > 127) ? -1 * this->_tile_index : this->_tile_index};

		// Setting pixel row's upper to the fetched tile byte.
		API::data_t tile_byte{0x00};

		// Fetching the tile according to the Y axis * 2 (2 bytes for each row, thus in jumps of 2)
		SANITY(this->_fifo.GetMemory().Read(lcdc_control.GetTileSelectOffset() + SIZE_OF_TILE * TILE_INDEX_OFFSET +
										    2*(this->_fifo.GetY() % PixelRow::PIXEL_COUNT) +
											static_cast<API::data_t>(lower), tile_byte), "Failed reading tile byte!");

		Message("TODO! Make this customizable");
		return {tile_byte, PixelSource::BGP};
	}

private:
	static constexpr std::size_t READ_DATA_0_CLOCKS{8};
	static constexpr std::size_t READ_DATA_1_CLOCKS{8};
	static constexpr std::size_t WAIT_FOR_FIFO_CLOCKS{8};
	static constexpr std::size_t FETCH_TILE_CLOCKS{8};
	static constexpr std::size_t TILES_IN_ROW{0x20};

private:
	State             _state{State::FETCH_TILE};
	API::address_t    _tile_offset{0x00};
	API::data_t       _tile_index{0x00};
	PixelRowContainer _pixel_row_container{};
	PixelFIFO&        _fifo;
	std::size_t       _clocks{0x00};
};
}

#endif // __LR35902_FETCHER_H__