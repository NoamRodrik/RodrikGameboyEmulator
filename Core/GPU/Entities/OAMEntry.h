/**
 * @file		OAMEntry.h
 * @author		Noam Rodrik
 * @description A single OAM entry for parsed information.
 */
#ifndef __LR35902_OAM_ENTRY_H__
#define __LR35902_OAM_ENTRY_H__

#include <API/Memory/Device/IMemoryDeviceAccess.h>
#include <Core/GPU/Entities/PixelRow.h>
#include <Core/GPU/Definitions.h>
#include <array>

namespace Core
{
class OAMEntry
{
public:
	enum class [[nodiscard]] Palette : API::data_t
	{
		OBP0 = 0x00,
		OBP1 = 0x01
	};

	enum class [[nodiscard]] Priority : API::data_t
	{
		ABOVE_BG = 0x00,
		BEHIND_BG_COLORS_1_TO_3 = 0x01
	};

public:
	OAMEntry(API::IMemoryDeviceAccess& memory_accessor, int32_t index);

public:
	/**
	 * Returns the pixel row of the sprite relative to the Y rendered.
	 * 
	 * @note This also includes wide tiles (8x16), controlled by the LSB.
	 */
	[[nodiscard]] PixelRow GetSpritePixelRow(std::size_t y) const;
	[[nodiscard]] const bool IsInScanline(std::size_t y) const;
	Message("Might need to be -8");
	[[nodiscard]] const API::data_t GetX() const { return this->_x - 16; }
	[[nodiscard]] const int32_t GetID() const { return this->_id; }
	[[nodiscard]] const auto GetPalette() const { return this->_palette; }
	[[nodiscard]] const auto GetPriority() const { return this->_priority; }

private:
	struct [[nodiscard]] OAMAttributes
	{
		constexpr OAMAttributes(const API::data_t data) :
			unused   {0x00},
			palette  ((data & 0b00010000) >> 4),
			x_flip   ((data & 0b00100000) >> 5),
			y_flip   ((data & 0b01000000) >> 6),
			priority ((data & 0b10000000) >> 7) {}

		const API::data_t unused : 4;
		const API::data_t palette : 1;
		const API::data_t x_flip : 1;
		const API::data_t y_flip : 1;
		const API::data_t priority : 1;

		constexpr operator API::data_t() const
		{
			const API::data_t RESULT = palette  << 4 |
									   x_flip   << 5 |
									   y_flip   << 6 |
									   priority << 7;

			return RESULT;
		}
	};

private:
	enum class [[nodiscard]] Flip : API::data_t
	{
		NO  = 0x00,
		YES = 0x01
	};

private:
	static constexpr std::size_t OAM_ENTRY_SIZE{0x04};
	static constexpr std::size_t Y_POSITION_INDEX{0x00};
	static constexpr std::size_t X_POSITION_INDEX{0x01};
	static constexpr std::size_t TILE_NUMBER_INDEX{0x02};
	static constexpr std::size_t ATTRIBUTES_INDEX{0x03};

private:
	API::IMemoryDeviceAccess& _memory_accessor;
	API::data_t _x{0x00};
	Flip        _x_flip{Flip::NO};
	API::data_t _y{0x00};
	Flip        _y_flip{Flip::NO};
	API::data_t _tile_index{0x00};
	Priority    _priority{Priority::ABOVE_BG};
	Palette     _palette{Palette::OBP0};
	int32_t     _id{0x00};
};
} // Core

#endif // __LR35902_OAM_ENTRY_H__