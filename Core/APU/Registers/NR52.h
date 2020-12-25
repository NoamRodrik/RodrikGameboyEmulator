/**
 * @file		NR52.h
 * @author		Noam Rodrik
 * @description LR35902's main apu NR52 definition header.
 */
#ifndef __LR35902_NR52_H__
#define __LR35902_NR52_H__

#include <Core/CPU/Registers/DeviceRegister.h>
#include <Core/APU/Definitions.h>
#include <Tools/Tools.h>

namespace Core
{
class [[nodiscard]] NR52 : public DeviceRegister<0xFF26>
{
public:
	using DeviceRegister::DeviceRegister;
	using DeviceRegister::operator=;
	using DeviceRegister::operator API::data_t;

public:
	[[nodiscard]] const bool IsSoundEnabled() const
	{
		return Tools::IsBitSet(operator API::data_t(), SOUND_ENABLED_BIT);
	}

	void SetStatus(const SoundChannel channel, const bool status)
	{
		API::data_t nr52{static_cast<API::data_t>(*this)};
		Tools::MutateBitByCondition(status, nr52, static_cast<size_t>(channel) - 1);
		*this = nr52;
	}

public:
	static constexpr auto SOUND_ENABLED_BIT{7};

public:
	static constexpr API::data_t NR52_DEFAULT_VALUE{0xF1};
};
} // Core

#endif // __LR35902_NR52_H__