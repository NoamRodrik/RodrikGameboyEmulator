/**
 * @file		SoundChannel1.cpp
 * @author		Noam Rodrik
 * @description Main logic of the APU's sound channel 1.
 */
#include <Core/APU/Waves/Implementation/SoundChannel1.h>
#include <Core/APU/Registers/NR51.h>
#include <Core/APU/Registers/NR14.h>
#include <Core/APU/Registers/NR13.h>
#include <Core/APU/Registers/NR12.h>
#include <Core/APU/Registers/NR11.h>
#include <Core/APU/Registers/NR10.h>
#include <Core/APU/Definitions.h>
#include <Core/CPU/Processor.h>

using namespace API;

namespace Core
{
void SoundChannel1::Restart()
{
	ASoundChannelSquare::SquareChannelRestart<NR11, NR14, NR12>();

	// ONLY SoundChannel1 has sweep!
	this->_sweep_time_elapsed = 0x00;
}

void SoundChannel1::UpdateSample()
{
	ASoundChannelSquare::SquareChannelUpdateSamples<NR11>();
}

void SoundChannel1::SweepTick()
{
	const NR10 nr10{};

	// We can't play sweep if it isn't on.
	if (nr10.GetSweepTime() != 0)
	{
		if (this->_sweep_time_elapsed != nr10.GetSweepTime())
		{
			this->_sweep_time_elapsed += 1;
		}
		else
		{
			// This will cause a possible overflow/underflow, but this is how the APU works.
			const uint8_t NEW_SWEEP = (this->GetFrequency() >> nr10.GetSweepShiftAmount()) * (nr10.IsSweepDecreasing() ? -1 : 1);

			// If we underflowed on decrease, that means the sweep has finished.
			if (nr10.IsSweepDecreasing() && NEW_SWEEP > this->GetFrequency())
			{
				this->_sweep_time_elapsed = 0;
			}
			// If we overflowed on increase => Stop SoundChannel1 from playing.
			else if (nr10.IsSweepIncreasing() && (NEW_SWEEP + this->GetFrequency()) > 2047)
			{
				this->SetEnabled(false);
			}
			else
			{
				// We can change the frequency here.
				this->SetFrequency(this->GetFrequency() + NEW_SWEEP);
				this->SetCycles(0x00);
				this->_sweep_time_elapsed = 0x00;

				// Set frequency in memory
				SANITY(Processor::GetInstance().GetMemory().WriteDirectly(NR13::NR13_ADDRESS, this->GetFrequency() & 0b11111111),
					   "Failed writing to NR13");
				SANITY(Processor::GetInstance().GetMemory().WriteDirectly(NR14::NR14_ADDRESS, (NR14{} & 0xF8) | ((this->GetFrequency() >> CHAR_BIT) & 0b111)),
					"Failed writing to NR13");
			}
		}
	}
}
} // Core