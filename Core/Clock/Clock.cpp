/**
 * @file		Clock.cpp
 * @author		Noam Rodrik
 * @description Main logic of the processor's clock
 */
#include <Core/CPU/Timer/Timer.h>
#include <Core/Clock/Clock.h>
#include <thread>

namespace Core
{
void Clock::SyncClock()
{
	static constexpr auto one_clock_period{lr35902_clock_period{1}};

	// Find out if the difference is less than one clock period
	while ((std::chrono::high_resolution_clock::now() - Clock::GetInstance().m_last_tick) < one_clock_period)
	{
		std::this_thread::sleep_for(one_clock_period);
	}

	// Use time_point_cast to convert (via truncation towards zero) back to
	// the "native" duration of high_resolution_clock
	Clock::GetInstance().m_last_tick =
		std::chrono::time_point_cast<std::chrono::high_resolution_clock::duration>(
			Clock::GetInstance().m_last_tick + one_clock_period);

	// Update all devices that need the clock that the clock has ticked.
	Clock::Update();
}

void Clock::Update()
{
	Timer::Tick();
}
} // Core