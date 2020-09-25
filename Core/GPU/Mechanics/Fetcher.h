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
		// Reset it to the first line of the SCY.
		this->_tile_offset = static_cast<API::address_t>((static_cast<double>(this->_fifo.scy) / static_cast<double>(0x08)) * TILES_IN_ROW);
	}

	constexpr void Clear()
	{
		this->_pixel_row_container.Clear();
		this->_state = State::FETCH_TILE;
	}

	constexpr void NextRowOffset()
	{
		// Complete the tile offset until the end of the row.
		this->_tile_offset += TILES_IN_ROW - (this->_tile_offset % TILES_IN_ROW);
	}

	constexpr bool Execute(std::size_t& io_clocks)
	{
		this->_clocks += io_clocks;

		switch (this->_state)
		{
			case (State::FETCH_TILE):
			{
				return this->FetchTile(io_clocks);
			}

			case (State::READ_DATA_0):
			{
				return this->ReadDataUpper(io_clocks);
			}

			case (State::READ_DATA_1):
			{
				return this->ReadDataLower(io_clocks);
			}

			case (State::WAIT_FOR_FIFO):
			{
				return this->WaitForFIFO(io_clocks);
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
	constexpr bool FetchTile(std::size_t& io_clocks)
	{
		constexpr std::size_t FETCH_TILE_CLOCKS{2};
		if (this->_clocks >= FETCH_TILE_CLOCKS)
		{
			auto lcdc_register{LCDC_Control{}};
			auto lcdc_control{static_cast<LCDC_Control::Control>(lcdc_register)};
			RET_FALSE_IF_FAIL(lcdc_control.Validate(), "Failed validating lcdc control");
			RET_FALSE_IF_FAIL(this->_fifo.GetMemory().Read(lcdc_control.GetBackgroundMapStart() + this->_tile_offset, this->_tile_index),
							  "Failed reading memory for canvas");

			io_clocks -= FETCH_TILE_CLOCKS;
			this->_clocks -= FETCH_TILE_CLOCKS;
			this->_state = State::READ_DATA_0;
		}

		return true;
	}

	/**
	 * Read data 0 state -> Read data 1 state
	 */
	constexpr bool ReadDataUpper(std::size_t& io_clocks)
	{
		constexpr std::size_t READ_DATA_0_CLOCKS{2};
		if (this->_clocks >= READ_DATA_0_CLOCKS)
		{
			this->_pixel_row_container.SetUpper(this->GetTileByte());
			this->_tile_index += 1;
			io_clocks -= READ_DATA_0_CLOCKS;
			this->_clocks -= READ_DATA_0_CLOCKS;
			this->_state = State::READ_DATA_1;
		}

		return true;
	}

	/**
	 * Read data 1 state -> Wait for FIFO state
	 */
	constexpr bool ReadDataLower(std::size_t& io_clocks)
	{
		constexpr std::size_t READ_DATA_1_CLOCKS{2};
		if (this->_clocks >= READ_DATA_1_CLOCKS)
		{
			this->_pixel_row_container.SetLower(this->GetTileByte(true));
			io_clocks -= READ_DATA_1_CLOCKS;
			this->_clocks -= READ_DATA_1_CLOCKS;
			this->_state = State::WAIT_FOR_FIFO;
		}

		return true;
	}

	/**
	 * Wait for FIFO state -> Fetch tile state
	 */
	constexpr bool WaitForFIFO(std::size_t& io_clocks)
	{
		constexpr std::size_t WAIT_FOR_FIFO_CLOCKS{2};
		if (this->_clocks >= WAIT_FOR_FIFO_CLOCKS)
		{
			if (this->_fifo.NeedsFill())
			{
				this->_fifo.Fill(_pixel_row_container);
				this->_pixel_row_container.Clear();
				io_clocks -= WAIT_FOR_FIFO_CLOCKS;
				this->_clocks -= WAIT_FOR_FIFO_CLOCKS;
				this->_state = State::FETCH_TILE;
			}
		}
		

		return true;
	}

private:
	constexpr API::data_t GetTileByte(bool lower = false)
	{
		auto lcdc_register{LCDC_Control{}};
		auto lcdc_control{static_cast<LCDC_Control::Control>(lcdc_register)};
		RET_FALSE_IF_FAIL(lcdc_control.Validate(), "Failed validating lcdc control");

		// Fetching address of the tile.
		constexpr API::address_t SIZE_OF_TILE{16};
		const API::address_t TILE_INDEX_OFFSET{(lcdc_control.IsSigned() && this->_tile_index > 127) ? static_cast<API::data_t>(-1 * this->_tile_index) : this->_tile_index};

		// Setting pixel row's upper to the fetched tile byte.
		API::data_t tile_byte{0x00};
		
		// Fetching the tile according to the Y axis * 2 (2 bytes for each row, thus in jumps of 2)
		SANITY(this->_fifo.GetMemory().Read(lcdc_control.GetTileSelectOffset() + SIZE_OF_TILE * TILE_INDEX_OFFSET +
										    static_cast<API::data_t>(lower) +
			   2*(this->_fifo.y % PixelRow::PIXEL_COUNT), tile_byte), "Failed reading tile byte!");

		return tile_byte;
	}

private:
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