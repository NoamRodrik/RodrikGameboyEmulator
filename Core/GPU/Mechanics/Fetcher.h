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
#include <Core/GPU/Registers/WY.h>
#include <Core/GPU/Registers/WX.h>
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
	void Reset()
	{
		this->_pixel_row_container.Clear();
		this->_state = State::FETCH_TILE;
		this->_tile_offset_x = this->_fifo.GetSCX();
	}

	bool Execute(std::size_t clocks)
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
				MAIN_LOG("Got into an impossible state in the fetcher: %02X!", this->_state);
				return false;
			}
		}
	}

private:
	/**
	 * Fetch tile state -> Read data 0 state
	 */
	const bool FetchTile()
	{
		static auto* vram_memory_ptr{static_cast<VideoRAM*>(this->_ppu.GetProcessor().GetMemory().GetDeviceAtAddress(VideoRAM::START_ADDRESS))->GetMemoryPointer()};

		if (this->_clocks >= FETCH_TILE_CLOCKS)
		{	
			const uint32_t TILE_OFFSET = (this->_fifo.GetY() / 8) * 32 + (this->_tile_offset_x / 8);
			this->_tile_offset_x = (this->_tile_offset_x + 8) % 0x100;
			Message("Undo this");
			/*if (static_cast<LCDC_Control::Control>(LCDC_Control{}).IsWindowEnabled() &&
				this->_fifo.GetY() >= static_cast<API::data_t>(WY{}) &&
				this->_fifo.GetX() >= static_cast<API::data_t>(WX{}))
			{
				Message("TODO! Make this better.");
				Message("Here we need to recalibrate the tile index with the X and Y offset of the window.");
				// If they're equal, clear fifo and restart fetcher
				if (this->_fifo.GetX() == static_cast<API::data_t>(WX{}))
				{
					this->_fifo.ResetNewLine();
					this->_fifo.SetX(WX{});
				}

				this->_tile_index = vram_memory_ptr[this->GetWindowMapStart() + TILE_OFFSET - VideoRAM::START_ADDRESS];
				this->_pixel_row_container.Initialize(PixelSource::WIN);
				this->_clocks -= FETCH_TILE_CLOCKS;
				this->_state = State::READ_DATA_0;
			}
			// If the window didn't succeed.
			else
			{*/
				this->_tile_index = vram_memory_ptr[this->GetBackgroundMapStart() + TILE_OFFSET - VideoRAM::START_ADDRESS];
				this->_pixel_row_container.Initialize(PixelSource::BGP);
				this->_clocks -= FETCH_TILE_CLOCKS;
				this->_state = State::READ_DATA_0;
			//}
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
			this->_pixel_row_container.SetUpper(this->GetUpperTileByte());
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
			this->_pixel_row_container.SetLower(this->GetLowerTileByte());
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
	const API::address_t GetBackgroundMapStart() const
	{
		return static_cast<LCDC_Control::Control>(LCDC_Control{}).GetBackgroundMapStart();
	}

	const API::address_t GetWindowMapStart() const
	{
		return static_cast<LCDC_Control::Control>(LCDC_Control{}).GetWindowMapStart();
	}

	const API::data_t GetUpperTileByte() const
	{
		return this->GetTileInformation(false);
	}

	const API::data_t GetLowerTileByte() const
	{
		return this->GetTileInformation(true);
	}

	const API::data_t GetTileInformation(bool lower = false) const
	{
		auto lcdc_control{static_cast<LCDC_Control::Control>(LCDC_Control{})};

		// Tiles are ordered in the tile map in 16 bytes, so 
		// in order for us to fetch the correct tile we need to
		// understand what kind of addressing this tile needs.
		// If the addressing is 0x8000, then the tile index offset
		// is simply 16*tile_index since 0x8010 is tile 1, 0x8020 is tile 2
		// and onwards.
		// If the addressing is 0x8800, then the base offset is 0x9000 and
		// the tile index can be a negative number, between [-128, 127] where
		// [-128, -1] are ordered ascending in memory and [0, 127] are ordered ascending.
		// -FF is -1, -FE is -2... For example -FF on addressing mode 0x8800 is:
		// 0x9000 + 16 * (-1) = 0x8FF0 which is the tile index.
		static constexpr API::address_t SIZE_OF_TILE{16};
		const int32_t TILE_INDEX_OFFSET = lcdc_control.GetTileSelectOffset() +
			SIZE_OF_TILE * ((lcdc_control.IsSigned() && (this->_tile_index > 127)) ? (static_cast<int32_t>(this->_tile_index) - 0x100) : this->_tile_index);

		// Once we know where the tile index is, we need to get the relevant row in the tile.
		// Fetching the tile according to the Y axis * 2 (2 bytes for each row, thus in jumps of 2)
		const API::data_t ROW_INDEX_OFFSET = 2 * (this->_fifo.GetY() % PixelRow::PIXEL_COUNT);

		// Setting pixel row's upper to the fetched tile byte.
		static auto* vram_memory_ptr{static_cast<VideoRAM*>(this->_ppu.GetProcessor().GetMemory().GetDeviceAtAddress(VideoRAM::START_ADDRESS))->GetMemoryPointer()};
		return vram_memory_ptr[(TILE_INDEX_OFFSET + ROW_INDEX_OFFSET + static_cast<API::data_t>(lower)) - VideoRAM::START_ADDRESS];
	}

private:
	static constexpr std::size_t READ_DATA_0_CLOCKS{8};
	static constexpr std::size_t READ_DATA_1_CLOCKS{8};
	static constexpr std::size_t WAIT_FOR_FIFO_CLOCKS{8};
	static constexpr std::size_t FETCH_TILE_CLOCKS{8};
	static constexpr std::size_t TILES_IN_ROW{0x20};

private:
	IPPU&             _ppu;
	PixelFIFO&        _fifo;
	State             _state{State::FETCH_TILE};
	API::address_t    _tile_offset_x{0x00};
	API::data_t       _tile_index{0x00};
	PixelRowContainer _pixel_row_container{};
	std::size_t       _clocks{0x00};
};
}

#endif // __LR35902_FETCHER_H__