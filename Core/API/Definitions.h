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

	/* RAM */
	static constexpr uint32_t MEMORY_SIZE = Tools::KilobitsToBits(std::numeric_limits<address_t>::digits);

	/* Devices on BUS */
	static constexpr uint8_t DEVICES_ON_BUS = 9;

} // Core

#endif // __API_DEFINITIONS_H__