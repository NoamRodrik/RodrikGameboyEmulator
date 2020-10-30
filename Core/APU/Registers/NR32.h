/**
 * @file		NR32.h
 * @author		Noam Rodrik
 * @description LR35902's main apu NR32 definition header.
 */
#ifndef __LR35902_NR32_H__
#define __LR35902_NR32_H__

#include <Core/CPU/Registers/DeviceRegister.h>

namespace Core
{
class [[nodiscard]] NR32 : public DeviceRegister<0xFF1C>
{
public:
	enum class [[nodiscard]] OutputLevel
	{
		MUTED = 0x00,
		FULL = 0x01,
		HALF = 0x02,
		QUARTER = 0x03
	};

public:
	using DeviceRegister::DeviceRegister;
	using DeviceRegister::operator=;
	using DeviceRegister::operator API::data_t;

public:
	const OutputLevel GetOutputLevel() const
	{
		return static_cast<OutputLevel>((static_cast<API::data_t>(*this) >> NR32_OUTPUT_LEVEL_BIT_INDEX) & 0b11);
	}

public:
	static constexpr API::data_t NR32_OUTPUT_LEVEL_BIT_INDEX{0x05};
	static constexpr API::data_t NR32_DEFAULT_VALUE{0x9F};
};
} // Core

#endif // __LR35902_NR32_H__