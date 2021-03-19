/**
 * @file		PixelRowContainer.h
 * @author		Noam Rodrik
 * @description LR35902 pixel row container for the FIFO.
 */
#ifndef __LR35902_PIXEL_ROW_CONTAINER_H__
#define __LR35902_PIXEL_ROW_CONTAINER_H__

#include <Core/GPU/Entities/PaletteMap.h>
#include <Core/GPU/Registers/OBP1.h>
#include <Core/GPU/Registers/OBP0.h>
#include <Core/GPU/Definitions.h>
#include <Core/CPU/Processor.h>
#include <API/Definitions.h>
#include <Tools/Tools.h>
#include "PixelRow.h"

namespace Core
{
class [[nodiscard]] PixelRowContainer
{
public:
	constexpr PixelRowContainer() = default;
	~PixelRowContainer() = default;

public:
	[[nodiscard]] constexpr int32_t GetNextPixelSource() const
	{
		return this->_pixel_source[0];
	}

	constexpr void InitializeSource(const int32_t pixel_source)
	{
		for (auto&& current_pixel_source : this->_pixel_source)
		{
			current_pixel_source = pixel_source;
		}
	}

	constexpr void SetPixelRow(const PixelRow pixel_row)
	{
		this->_current_pixel_row = pixel_row;
		this->_pixel_row_index = PixelRow::PIXEL_COUNT;
	}

	/**
	 * No pixels left in the pixel container.
	 */
	[[nodiscard]] constexpr bool IsEmpty() const
	{
		return this->_pixel_row_index == 0;
	}

	/**
	 * Fetching left-most pixel from the pixel row.
	 */
	[[nodiscard]] constexpr std::pair<int32_t, PaletteColor> GetNextPixel()
	{
		SANITY(!this->IsEmpty() && this->_pixel_row_index <= PixelRow::PIXEL_COUNT,
			"Got invalid values for pixel row index");
		const auto FETCHED_PIXEL_SOURCE{this->_pixel_source[0]};

		// Shifting all the pixel sources to the left
		for (size_t pixel_source_index = 0; pixel_source_index < PixelRow::PIXEL_COUNT - 1; ++pixel_source_index)
		{
			this->_pixel_source[pixel_source_index] = this->_pixel_source[pixel_source_index + 1];
		}
		this->_pixel_source[PixelRow::PIXEL_COUNT - 1] = BGP_PIXEL;

		this->_pixel_row_index -= 1;
		return {FETCHED_PIXEL_SOURCE, this->_current_pixel_row.StealTopColor()};
	}

	constexpr void SetLastPixel(int32_t source, PaletteColor pixel_color)
	{
		SANITY(this->EmptyBitsAmount() >= 1, "Too full");
		this->_pixel_source[this->_pixel_row_index] = source;
		this->_pixel_row_index += 1;
		this->_current_pixel_row.SetBottomColor(pixel_color);
	}

	constexpr void Clear()
	{
		this->SetPixelRow(PixelRow{});
		this->InitializeSource(BGP_PIXEL);
		this->_pixel_row_index = 0;
	}

	constexpr void RemoveTopColors(const std::size_t amount)
	{
		for (std::size_t index = 0; index < amount; ++index)
		{
			this->_current_pixel_row.StealTopColor();
		}

		this->_pixel_row_index -= amount;
	}

	[[nodiscard]] constexpr std::size_t EmptyBitsAmount()
	{
		return PixelRow::PIXEL_COUNT - this->_pixel_row_index;
	}

	void ShiftRightPixels(const std::size_t count, const PaletteColor palette_color)
	{
		this->_current_pixel_row.ShiftRightPixels(count, palette_color);
	}

	void ShiftLeftPixels(const std::size_t count, const PaletteColor palette_color)
	{
		this->_current_pixel_row.ShiftLeftPixels(count, palette_color);
	}

	void Combine(const PixelRowContainer& other, OAMEntryManager& entries)
	{
		for (size_t pixel_index = 0; pixel_index < PixelRow::PIXEL_COUNT; ++pixel_index)
		{
			const auto MY_SOURCE{this->_pixel_source[pixel_index]};
			const auto HIS_SOURCE{other._pixel_source[pixel_index]};
			bool swap{false};

			if (MY_SOURCE == HIS_SOURCE)
			{
				const auto& HIS_SPRITE{entries.GetSprite(HIS_SOURCE)};
				if (HIS_SPRITE->GetPalette() == OAMEntry::Palette::OBP0)
				{
					swap = other._current_pixel_row.GetColorAtIndex(pixel_index) != PaletteMap::TransparentColor<OBP0>();
				}
				else
				{
					swap = other._current_pixel_row.GetColorAtIndex(pixel_index) != PaletteMap::TransparentColor<OBP1>();
				}
			}
			else if (MY_SOURCE == BGP_PIXEL)
			{
				// I'm a background pixel.
				const auto& HIS_SPRITE{entries.GetSprite(HIS_SOURCE)};

				if (HIS_SPRITE->GetPalette() == OAMEntry::Palette::OBP0)
				{
					swap = other._current_pixel_row.GetColorAtIndex(pixel_index) != PaletteMap::TransparentColor<OBP0>();
				}
				else
				{
					swap = other._current_pixel_row.GetColorAtIndex(pixel_index) != PaletteMap::TransparentColor<OBP1>();
				}

				if (HIS_SPRITE->GetPriority() == OAMEntry::Priority::BEHIND_BG_COLORS_1_TO_3)
				{
					swap = swap && this->_current_pixel_row.GetColorAtIndex(pixel_index) == PaletteMap::ColorOf<BGP>(PaletteColor::FIRST_PALETTE);
				}
			}

			if (swap)
			{
				this->_pixel_source[pixel_index] = HIS_SOURCE;
				this->_current_pixel_row.SetColorAtIndex(pixel_index, other._current_pixel_row.GetColorAtIndex(pixel_index));
			}
		}
	}

private:
	PixelRow    _current_pixel_row{};
	std::size_t _pixel_row_index{0};
	int32_t     _pixel_source[PixelRow::PIXEL_COUNT]{BGP_PIXEL,
												     BGP_PIXEL,
													 BGP_PIXEL,
													 BGP_PIXEL,
													 BGP_PIXEL,
													 BGP_PIXEL,
													 BGP_PIXEL,
													 BGP_PIXEL};
};
} // Core

#endif // __LR35902_PIXEL_ROW_CONTAINER_H__