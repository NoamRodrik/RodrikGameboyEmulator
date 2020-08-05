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

	/* Special Registers */
	static constexpr address_t OVERRIDE_BOOTROM_ADDRESS{0xFF50};
} // Core

#endif // __API_DEFINITIONS_H__