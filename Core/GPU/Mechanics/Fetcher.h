/**
 * @file		Fetcher.h
 * @author		Noam Rodrik
 * @description LR35902 fetcher object for pixel rows
 */
#ifndef __LR35902_FETCHER_H__
#define __LR35902_FETCHER_H__

#include <Core/GPU/Entities/PixelRowContainer.h>
#include <Core/GPU/Mechanics/OAMEntryManager.h>
#include <Core/GPU/Registers/LCDC_Control.h>
#include <Core/GPU/Mechanics/PixelFIFO.h>
#include <Core/Bus/Devices/VideoRAMDevice.h>
#include <Core/GPU/Registers/SCY.h>
#include <Core/GPU/Registers/WY.h>
#include <Core/GPU/Registers/WX.h>
#include <API/Definitions.h>
#include <Tools/Tools.h>
#include <algorithm>

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
class [[nodiscard]] Fetcher
{
public:
	Fetcher(PixelFIFO& fifo, IPPU& ppu, OAMEntryManager& entry_manager) : _fifo{fifo}, _ppu{ppu}, _entry_manager{entry_manager} {}
	~Fetcher() = default;

private:
	enum class [[nodiscard]] State
	{
		FETCH_TILE = 0x00,
		READ_DATA_0 = 0x01,
		READ_DATA_1 = 0x02,
		WAIT_FOR_FIFO = 0x03
	};

public:
	PixelFIFO& GetPixelFIFO()
	{
		return this->_fifo;
	}

	void Reset()
	{
		this->_pixel_row_container.Clear();
		this->_state = State::FETCH_TILE;
		this->_tile_offset_x = this->_fifo.GetSCX();
		this->_window_tile_offset_x = WX{} - 7;

		// Reloading the sprites
		std::fill(this->_sprites.begin(), this->_sprites.end(), nullptr);
		this->_sprites = this->_entry_manager.GetSpritesInLine(this->_fifo.GetY());
	}

	[[nodiscard]] const bool Execute(std::size_t clocks)
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
				LOG("Got into an impossible state in the fetcher: %02X!", this->_state);
				return false;
			}
		}
	}

private:
	/**
	 * Fetch tile state -> Read data 0 state
	 */
	[[nodiscard]] const bool FetchTile()
	{
		if (this->_clocks >= FETCH_TILE_CLOCKS)
		{	
			const API::data_t WINDOW_X{WX{}};
			const API::data_t WINDOW_Y{WY{}};
			const API::data_t SCREEN_X = static_cast<API::data_t>(GetWrappedAroundDistance(this->_fifo.GetX(), this->_fifo.GetSCX()));
			const API::data_t SCREEN_Y = static_cast<API::data_t>(GetWrappedAroundDistance(this->_fifo.GetY(), this->_fifo.GetSCY()));

			if (static_cast<LCDC_Control::Control>(LCDC_Control{}).IsWindowEnabled() &&
				SCREEN_Y >= WINDOW_Y && SCREEN_X >= (WINDOW_X - 7))
			{
				// If they're equal, clear fifo
				if (this->_window_tile_offset_x == (WINDOW_X - 7))
				{
					this->_fifo.Reset();
				}

				const uint32_t TILE_OFFSET = ((SCREEN_Y - WINDOW_Y) / 8) * 32 + ((this->_window_tile_offset_x - (WINDOW_X - 7)) / 8);
				this->_window_tile_offset_x = (this->_window_tile_offset_x + 8) % 0x100;
				RET_FALSE_IF_FAIL(this->_ppu.GetProcessor().GetMemory().Read(this->GetWindowMapStart() + TILE_OFFSET, this->_tile_index), "Failed fetching tile index");
				this->_pixel_row_container.InitializeSource(BGP_PIXEL);
			}
			// If the window didn't succeed.
			else
			{
				const uint32_t TILE_OFFSET = (this->_fifo.GetY() / 8) * 32 + (this->_tile_offset_x / 8);
				this->_tile_offset_x = (this->_tile_offset_x + 8) % 0x100;
				RET_FALSE_IF_FAIL(this->_ppu.GetProcessor().GetMemory().Read(this->GetBackgroundMapStart() + TILE_OFFSET, this->_tile_index), "Failed fetching tile index");
				this->_pixel_row_container.InitializeSource(BGP_PIXEL);
			}

			this->_clocks -= FETCH_TILE_CLOCKS;
			this->_state = State::READ_DATA_0;
		}

		return true;
	}

	/**
	 * Read data 0 state -> Read data 1 state
	 */
	[[nodiscard]] const bool ReadDataUpper()
	{
		if (this->_clocks >= READ_DATA_0_CLOCKS)
		{
			this->_building_pixel_row.SetUpper(this->GetUpperTileByte());

			this->_clocks -= READ_DATA_0_CLOCKS;
			this->_state = State::READ_DATA_1;
		}

		return true;
	}

	/**
	 * Read data 1 state -> Wait for FIFO state
	 */
	[[nodiscard]] const bool ReadDataLower()
	{
		if (this->_clocks >= READ_DATA_1_CLOCKS)
		{
			this->_building_pixel_row.SetLower(this->GetLowerTileByte());

			this->_clocks -= READ_DATA_1_CLOCKS;
			this->_state = State::WAIT_FOR_FIFO;
		}

		return true;
	}

	/**
	 * Wait for FIFO state -> Fetch tile state
	 */
	[[nodiscard]] const bool WaitForFIFO()
	{
		if (this->_clocks >= WAIT_FOR_FIFO_CLOCKS)
		{
			// Will anyways use up clocks
			this->_clocks -= WAIT_FOR_FIFO_CLOCKS;

			if (this->_fifo.NeedsFill())
			{
				this->_pixel_row_container.SetPixelRow(this->_building_pixel_row);

				OAMEntry* entry{this->SpriteToDraw()};
				if (entry != nullptr)
				{
					const API::data_t SCREEN_X = static_cast<API::data_t>(GetWrappedAroundDistance(this->_fifo.GetX(), this->_fifo.GetSCX()));
					const API::data_t SCREEN_Y = static_cast<API::data_t>(GetWrappedAroundDistance(this->_fifo.GetY(), this->_fifo.GetSCY()));
					PixelRowContainer pixel_row_container{};
					PixelRow sprite_pixel_row{entry->GetSpritePixelRow(SCREEN_Y)};
					pixel_row_container.SetPixelRow(sprite_pixel_row);
					pixel_row_container.InitializeSource(entry->GetID());

					// Shift N transparent pixels from the left to the right.
					const PaletteColor TRANSPARENT_COLOR = 
						static_cast<PaletteColor>(entry->GetPalette() == OAMEntry::Palette::OBP0 ?
						PaletteMap::TransparentColor<OBP0>() : PaletteMap::TransparentColor<OBP1>());

					if (SCREEN_X != entry->GetX())
					{
						PixelRowContainer lower_half{};
						lower_half.SetPixelRow(sprite_pixel_row);
						lower_half.InitializeSource(entry->GetID());
						lower_half.ShiftRightPixels(PixelRow::PIXEL_COUNT - (SCREEN_X - entry->GetX()), TRANSPARENT_COLOR);
						this->_fifo._lower_row.Combine(lower_half, this->_entry_manager);
						pixel_row_container.ShiftLeftPixels(SCREEN_X - entry->GetX(), TRANSPARENT_COLOR);
					}

					// We have an entry to draw.
					this->_pixel_row_container.Combine(pixel_row_container, this->_entry_manager);
				}

				this->_fifo.Fill(this->_pixel_row_container);
				this->_pixel_row_container.Clear();
				this->_state = State::FETCH_TILE;
			}
		}

		return true;
	}

private:
	[[nodiscard]] OAMEntry* SpriteToDraw()
	{
		if (static_cast<LCDC_Control::Control>(LCDC_Control{}).IsSpriteEnabled())
		{
			auto iterator{std::find_if(this->_sprites.begin(), this->_sprites.end(), [this](const std::shared_ptr<OAMEntry>& entry)
			{
				const int SCREEN_X{GetWrappedAroundDistance(this->GetPixelFIFO().GetX(), this->GetPixelFIFO().GetSCX())};
				if (entry.get() != nullptr)
				{
					const int DISTANCE{SCREEN_X - entry->GetX()};
					return DISTANCE >= 0 && DISTANCE < PixelRow::PIXEL_COUNT;
				}

				return false;
			})};

			return (iterator != this->_sprites.end()) ? iterator->get() : nullptr;
		}

		return nullptr;
	}

	[[nodiscard]] const API::address_t GetBackgroundMapStart() const
	{
		return static_cast<LCDC_Control::Control>(LCDC_Control{}).GetBackgroundMapStart();
	}

	[[nodiscard]] const API::address_t GetWindowMapStart() const
	{
		return static_cast<LCDC_Control::Control>(LCDC_Control{}).GetWindowMapStart();
	}

	[[nodiscard]] const API::data_t GetUpperTileByte() const
	{
		return this->GetTileInformation(false);
	}

	[[nodiscard]] const API::data_t GetLowerTileByte() const
	{
		return this->GetTileInformation(true);
	}

	[[nodiscard]] const API::data_t GetTileInformation(bool lower = false) const
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
		API::data_t result{0x00};
		SANITY(this->_ppu.GetProcessor().GetMemory().Read(TILE_INDEX_OFFSET + ROW_INDEX_OFFSET + static_cast<API::data_t>(lower), result), "Failed fetching tile");
		return result;
	}

private:
	static constexpr std::size_t READ_DATA_0_CLOCKS{8};
	static constexpr std::size_t READ_DATA_1_CLOCKS{8};
	static constexpr std::size_t WAIT_FOR_FIFO_CLOCKS{8};
	static constexpr std::size_t FETCH_TILE_CLOCKS{8};
	static constexpr std::size_t TILES_IN_ROW{0x20};

private:
	IPPU&							   _ppu;
	PixelFIFO&						   _fifo;
	State							   _state{State::FETCH_TILE};
	API::address_t					   _tile_offset_x{0x00};
	API::address_t					   _window_tile_offset_x{0x00};
	API::data_t						   _tile_index{0x00};
	PixelRowContainer				   _pixel_row_container{};
	PixelRow                           _building_pixel_row{};
	std::size_t						   _clocks{0x00};
	OAMEntryManager&                   _entry_manager;
	OAMEntryManager::sprites_in_line_t _sprites{};
};
} // Core

#endif // __LR35902_FETCHER_H__