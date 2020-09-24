/**
 * @file		Fetcher.h
 * @author		Noam Rodrik
 * @description LR35902 fetcher object for pixel rows
 */
#ifndef __LR35902_FETCHER_H__
#define __LR35902_FETCHER_H__

#include <Core/GPU/Entities/PixelRowContainer.h>
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
	constexpr bool LoadedPixelRow() const
	{
		return this->_loaded_pixel_row;
	}

	constexpr bool Execute()
	{
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
				LOG("Got into an impossible state in the fetcher!");
				return false;
			}
		}
	}

private:
	/**
	 * Fetch tile state -> Read data 0 state
	 */
	bool FetchTile()
	{
		Message("TODO!");
		this->_state = State::READ_DATA_0;
		return true;
	}

	/**
	 * Read data 0 state -> Read data 1 state
	 */
	bool ReadDataUpper()
	{
		Message("TODO!");
		this->_state = State::READ_DATA_1;
		return true;
	}

	/**
	 * Read data 1 state -> Wait for FIFO state
	 */
	bool ReadDataLower()
	{
		Message("TODO!");
		this->_state = State::WAIT_FOR_FIFO;
		return true;
	}

	/**
	 * Wait for FIFO state -> Fetch tile state
	 */
	bool WaitForFIFO()
	{
		if (this->_fifo.NeedsFill())
		{
			this->_fifo.Fill(_pixel_row_container);
			this->_pixel_row_container.Clear();
			this->_state = State::FETCH_TILE;
		}

		return true;
	}

private:
	State             _state{State::FETCH_TILE};
	API::address_t    _fetcher_tile{0x00};
	PixelRowContainer _pixel_row_container{};
	bool              _loaded_pixel_row{false};
	PixelFIFO&        _fifo;
};
}

#endif // __LR35902_FETCHER_H__