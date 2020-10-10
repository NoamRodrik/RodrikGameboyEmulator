/**
 * @file		APU.cpp
 * @author		Noam Rodrik
 * @description Main logic of the APU
 */
#include <Core/APU/APU.h>
#include <Core/APU/Definitions.h>

namespace Core
{
void APU::Clock(std::size_t clocks)
{
	this->_wave_controller.Clock(static_cast<API::data_t>(clocks));
}
} // Core