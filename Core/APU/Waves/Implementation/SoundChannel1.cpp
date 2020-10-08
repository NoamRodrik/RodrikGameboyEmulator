/**
 * @file		SoundChannel1.cpp
 * @author		Noam Rodrik
 * @description Main logic of the APU's sound channel 1.
 */
#include <Core/APU/Waves/Implementation/SoundChannel1.h>
#include <Core/APU/Registers/NR51.h>
#include <Core/APU/Definitions.h>

using namespace API;

namespace Core
{
const float_t SoundChannel1::Sample(const float_t time) const
{
	// Square wave
	float_t sin_wave_a{0x00};
	float_t sin_wave_b{0x00};
	const float_t PHASE_DIFFERENCE{this->_duty_cycle * 2.0f * PI<float_t>};

	for (float_t harmonic_index = 0x00; harmonic_index < this->_harmonics; ++harmonic_index)
	{
		const float_t HARMONIC_DISTRIBUTION{harmonic_index * this->GetFrequency() * 2.0f * PI<float_t> * time};
		sin_wave_a += sin(HARMONIC_DISTRIBUTION) / harmonic_index;
		sin_wave_b += sin(HARMONIC_DISTRIBUTION - PHASE_DIFFERENCE * harmonic_index) / harmonic_index;
	}

	return (2.0f / PI<float_t>) * (sin_wave_a - sin_wave_b);
}
} // Core