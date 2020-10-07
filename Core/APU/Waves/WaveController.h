/**
 * @file		WaveController.h
 * @author		Noam Rodrik
 * @description The implementation for LR35902's synthesizer.
 */
#ifndef __CORE_WAVE_CONTROLLER_H__
#define __CORE_WAVE_CONTROLLER_H__

#include <Core/APU/Waves/ISoundChannel.h>
#include <Core/APU/Definitions.h>
#include <API/Definitions.h>
#include <cstddef>
#include <memory>
#include <array>

namespace Core
{
class WaveController
{
public:
	WaveController();
	~WaveController() = default;

private:
	std::array<std::unique_ptr<ISoundChannel>, API::AUDIO_CHANNELS_AMOUNT> _waves{};
};
} // Core

#endif // __CORE_WAVE_CONTROLLER_H__