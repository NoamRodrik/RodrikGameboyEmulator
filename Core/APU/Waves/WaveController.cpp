/**
 * @file		WaveController.cpp
 * @author		Noam Rodrik
 * @description Main logic of the APU's wave controller.
 */
#include <Core/APU/Waves/WaveController.h>
#include <Core/APU/Definitions.h>
#include <Core/APU/Waves/Implementation/SoundChannelNoise.h>
#include <Core/APU/Waves/Implementation/SoundChannel1.h>
#include <Core/APU/Waves/Implementation/SoundChannel2.h>
#include <Core/APU/Waves/Implementation/SoundChannel3.h>

namespace Core
{
WaveController::WaveController()
{
	// Initializing the channels
	this->_waves[0] = std::make_unique<SoundChannel1>();
	this->_waves[1] = std::make_unique<SoundChannel2>();
	this->_waves[2] = std::make_unique<SoundChannel3>();
	this->_waves[3] = std::make_unique<SoundChannelNoise>();
}
} // Core