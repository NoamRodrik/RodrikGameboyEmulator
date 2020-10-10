/**
 * @file		APU.h
 * @author		Noam Rodrik
 * @description The implementation for LR35902's APU.
 */
#ifndef __CORE_APU_H__
#define __CORE_APU_H__

#include <Core/APU/Waves/WaveController.h>
#include <Core/APU/Definitions.h>
#include <API/StaticInstance.h>
#include <API/Definitions.h>
#include <cstddef>

namespace Core
{
class [[nodiscard]] APU : public API::StaticInstance<APU>
{
public:
	APU() = default;
	~APU() = default;

public:
	void Clock(std::size_t clocks);

	[[nodiscard]] WaveController& GetOscillator()
	{
		return APU::GetInstance()._wave_controller;
	}

private:
	WaveController _wave_controller{};
};
} // Core

#endif // __CORE_APU_H__