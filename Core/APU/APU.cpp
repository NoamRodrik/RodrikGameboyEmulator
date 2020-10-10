/**
 * @file		APU.cpp
 * @author		Noam Rodrik
 * @description Main logic of the APU
 */
#include <Core/APU/Registers/NR52.h>
#include <Core/APU/Definitions.h>
#include <Core/APU/APU.h>

namespace Core
{
void APU::Clock(std::size_t clocks)
{
	if (NR52{}.IsSoundEnabled())
	{
		this->_wave_controller.Clock(static_cast<API::data_t>(clocks));
	}
}
} // Core