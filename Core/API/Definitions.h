/**
 * @file		Definitions.h
 * @author		Noam Rodrik
 * @description API main definition header.
 */
#ifndef __API_DEFINITIONS_H__
#define __API_DEFINITIONS_H__

#include <Tools/Tools.h>
#include <stdint.h>
#include <limits>

namespace Core
{
	/* Necessary types for the CPU */
	using address_t = uint16_t;
	using data_t = uint8_t;
	using r8_t = int8_t;

	/* Necessary switch */
	static constexpr data_t OFF = 0;
	static constexpr data_t ON  = 1;

	/* RAM */
	static constexpr uint32_t MEMORY_SIZE = Tools::KilobitsToBits(std::numeric_limits<address_t>::digits);

	/* Main memory constants */
	static constexpr address_t ZERO_PAGE_ADDRESS{0xFF00};

	/* Devices on BUS */
	static constexpr uint8_t DEVICES_ON_BUS = 10;

	/* Interrupts */
	static constexpr uint8_t INTERRUPT_COUNT = 5;

	/* Joypad */
	static constexpr address_t JOYPAD_REGISTER_ADDRESS{0xFF00};

	/* Serial */
	static constexpr address_t SERIAL_TRANSFER_DATA{0xFF01};
	static constexpr address_t SERIAL_TRANSFER_CONTROL{0xFF02};
	static constexpr data_t SERIAL_TRANSFER_START{0x01};
	static constexpr data_t SERIAL_TRANSFER_CLOCK_SOURCE{0x80};

	/* Timers */
	static constexpr address_t DIVIDER_REGISTER_ADDRESS{0xFF04};
	static constexpr address_t TIMER_COUNTER_ADDRESS{0xFF05};
	static constexpr address_t TIMER_MODULO_ADDRESS{0xFF06};
	static constexpr address_t TIMER_CONTROL_ADDRESS{0xFF07};

	/* Audio Processing and Graphics Processing */
	static constexpr address_t NR10_ADDRESS{0xFF10};
	static constexpr address_t NR11_ADDRESS{0xFF11};
	static constexpr address_t NR12_ADDRESS{0xFF12};
	static constexpr address_t NR14_ADDRESS{0xFF14};
	static constexpr address_t NR21_ADDRESS{0xFF16};
	static constexpr address_t NR22_ADDRESS{0xFF17};
	static constexpr address_t NR24_ADDRESS{0xFF19};
	static constexpr address_t NR30_ADDRESS{0xFF1A};
	static constexpr address_t NR31_ADDRESS{0xFF1B};
	static constexpr address_t NR32_ADDRESS{0xFF1C};
	static constexpr address_t NR33_ADDRESS{0xFF1E};
	static constexpr address_t NR41_ADDRESS{0xFF20};
	static constexpr address_t NR42_ADDRESS{0xFF21};
	static constexpr address_t NR43_ADDRESS{0xFF22};
	static constexpr address_t NR44_ADDRESS{0xFF23};
	static constexpr address_t NR50_ADDRESS{0xFF24};
	static constexpr address_t NR51_ADDRESS{0xFF25};
	static constexpr address_t NR52_ADDRESS{0xFF26};
	static constexpr address_t LCDC_ADDRESS{0xFF40};
	static constexpr address_t SCY_ADDRESS {0xFF42};
	static constexpr address_t SCX_ADDRESS {0xFF43};
	static constexpr address_t LYC_ADDRESS {0xFF45};
	static constexpr address_t BGP_ADDRESS {0xFF47};
	static constexpr address_t OBP0_ADDRESS{0xFF48};
	static constexpr address_t OBP1_ADDRESS{0xFF49};
	static constexpr address_t WY_ADDRESS  {0xFF4A};
	static constexpr address_t WX_ADDRESS  {0xFF4B};
	static constexpr address_t NR10_DEFAULT_VALUE{0x80};
	static constexpr address_t NR11_DEFAULT_VALUE{0xBF};
	static constexpr address_t NR12_DEFAULT_VALUE{0xF3};
	static constexpr address_t NR14_DEFAULT_VALUE{0xBF};
	static constexpr address_t NR21_DEFAULT_VALUE{0xF3};
	static constexpr address_t NR22_DEFAULT_VALUE{0x00};
	static constexpr address_t NR24_DEFAULT_VALUE{0xBF};
	static constexpr address_t NR30_DEFAULT_VALUE{0x7F};
	static constexpr address_t NR31_DEFAULT_VALUE{0xFF};
	static constexpr address_t NR32_DEFAULT_VALUE{0x9F};
	static constexpr address_t NR33_DEFAULT_VALUE{0xBF};
	static constexpr address_t NR41_DEFAULT_VALUE{0xFF};
	static constexpr address_t NR42_DEFAULT_VALUE{0x00};
	static constexpr address_t NR43_DEFAULT_VALUE{0x00};
	static constexpr address_t NR44_DEFAULT_VALUE{0xBF};
	static constexpr address_t NR50_DEFAULT_VALUE{0x77};
	static constexpr address_t NR51_DEFAULT_VALUE{0xF3};
	static constexpr address_t NR52_DEFAULT_VALUE{0xF1};
	static constexpr address_t LCDC_DEFAULT_VALUE{0x91};
	static constexpr address_t SCY_DEFAULT_VALUE {0x00};
	static constexpr address_t SCX_DEFAULT_VALUE {0x00};
	static constexpr address_t LYC_DEFAULT_VALUE {0x00};
	static constexpr address_t BGP_DEFAULT_VALUE {0xFC};
	static constexpr address_t OBP0_DEFAULT_VALUE{0xFF};
	static constexpr address_t OBP1_DEFAULT_VALUE{0xFF};
	static constexpr address_t WY_DEFAULT_VALUE  {0x00};
	static constexpr address_t WX_DEFAULT_VALUE  {0x00};

	/* Special Registers */
	static constexpr address_t OVERRIDE_BOOTROM_ADDRESS{0xFF50};
} // Core

#endif // __API_DEFINITIONS_H__