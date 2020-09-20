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
		// 0: Off
		static constexpr API::data_t BACKGROUND_OFF{0x00};
		// 1: On
		static constexpr API::data_t BACKGROUND_ON{0x01};
		API::data_t background_enable : 1;

		// OBJ_EN
		// 0: Off
		static constexpr API::data_t SPRITE_OFF{0x00};
		// 1: On
		static constexpr API::data_t SPRITE_ON{0x01};
		API::data_t sprite_enable : 1;

		// OBJ_SIZE
		// 0: 8 * 8
		static constexpr API::data_t SPRITE_SIZE_8_BY_8{0x00};
		// 1: 8 * 16 (width * height)
		static constexpr API::data_t SPRITE_SIZE_8_BY_16{0x01};
		API::data_t sprite_size : 1;

		// BG_MAP
		// 0: $9800 - $9BFF
		static constexpr API::data_t BACKGROUND_MAP_9800_9BFF{0x00};
		// 1: $9C00 - $9FFF
		static constexpr API::data_t BACKGROUND_MAP_9C00_9FFF{0x01};
		API::data_t background_map_select : 1;

		// TILE_SEL
		// 0: $8800 - $97FF
		static constexpr API::data_t TILE_MAP_SELECT_8800_97FF{0x00};
		// 1: $8000 - $8FFF <- Same area as OBJ
		static constexpr API::data_t TILE_MAP_SELECT_8000_8FFF{0x01};
		API::data_t tile_select : 1;

		// WIN_EN
		// 0: Off
		static constexpr API::data_t WINDOW_OFF{0x00};
		// 1: On
		static constexpr API::data_t WINDOW_ON{0x01};
		API::data_t window_enable : 1;

		// WIN_MAP
		// 0: $9800 - $9BFF
		static constexpr API::data_t WINDOWS_MAP_9800_9BFF{0x00};
		// 1: $9C00 - $9FFF
		static constexpr API::data_t WINDOWS_MAP_9C00_9FFF{0x01};
		API::data_t window_map_select : 1;

		// LCD_EN
		// 0: Stop completely (no picture on screen)
		static constexpr API::data_t LCD_STOP{0x00};
		// 1: operation
		static constexpr API::data_t LCD_OPERATION{0x01};
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
	/**
	 * Mutate/Access the LCDC via a bit field structure.
	 */
	operator Control()
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