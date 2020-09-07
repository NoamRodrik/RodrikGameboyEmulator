/**
 * @file		LCDC_Control.h
 * @author		Noam Rodrik
 * @description LR35902's main gpu lcdc control definition header.
 */
#ifndef __LR35902_LCDC_CONTROL_H__
#define __LR35902_LCDC_CONTROL_H__

#include <Core/CPU/Registers/MemoryRegister.h>

namespace Core
{
class LCDC_Control : public MemoryRegister<API::data_t>
{
public:
	constexpr LCDC_Control() : MemoryRegister{LCDC_ADDRESS} {}
	LCDC_Control(const API::data_t value) : MemoryRegister{LCDC_ADDRESS} { *this = value; }

public:
	using MemoryRegister::operator=;
	using MemoryRegister::operator API::data_t;

public:
	struct Control
	{
		// BG_EN
		API::data_t background_enable : 1;

		// OBJ_EN
		API::data_t sprite_enable : 1;

		// OBJ_SIZE
		// 0: 8 * 8
		// 1: 8 * 16 (width * height)
		API::data_t sprite_size : 1;

		// BG_MAP
		// 0: $9800  -$9BFF
		// 1: $9C00 - $9FFF
		API::data_t background_map_select : 1;

		// TILE_SEL
		// 0: $8800  -$97FF
		// 1: $8000 - $8FFF < -Same area as OBJ
		API::data_t tile_select : 1;

		// WIN_EN
		API::data_t window_enable : 1;

		// WIN_MAP
		// 0: $9800 - $9BFF
		// 1: $9C00 - $9FFF
		API::data_t window_map_select : 1;

		// LCD_EN
		API::data_t lcd_operation : 1;

		constexpr operator API::data_t() const
		{
			const API::data_t DATA = lcd_operation << 7 |
									 window_map_select << 6 |
									 window_enable << 5 |
									 tile_select << 4 |
									 background_map_select << 3 |
									 sprite_size << 2 |
									 sprite_enable << 1 |
									 background_enable;
			return DATA;
		}
	};

	static_assert(sizeof(Control) == sizeof(API::data_t),
				  "Control too big! Supposed to of size data_t!");

public:
	const Control GetSettings() const
	{
		return {this->operator API::data_t()};
	}

	MemoryRegister& operator=(const Control other)
	{
		return this->operator=(static_cast<const API::data_t>(other));
	}

public:
	static constexpr API::address_t LCDC_ADDRESS{0xFF40};
	static constexpr API::address_t LCDC_DEFAULT_VALUE{0x91};
};
}

#endif // __LR35902_LCDC_CONTROL_H__