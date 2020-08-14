/**
 * @file		Clock.cpp
 * @author		Noam Rodrik
 * @description Main logic of the processor's clock
 */
#include <Core/CPU/Timers/Timer.h>
#include <Core/Clock/Clock.h>
#include <thread>

namespace Core
{
void Clock::SyncClock()
{
	// Sleep until one tick has passed.
	std::this_thread::sleep_until(Clock::GetInstance().m_last_tick);

	// Use time_point_cast to convert (via truncation towards zero) back to
	// the "native" duration of high_resolution_clock
	Clock::GetInstance().m_last_tick =
		std::chrono::time_point_cast<clock::duration>(Clock::GetInstance().m_last_tick + one_clock_period);

	// Update all devices that need the clock that the clock has ticked.
	Clock::Update();
}

void Clock::Update()
{
	Timer::Tick();
}
} // Core