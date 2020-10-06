/**
 * @file		Clock.cpp
 * @author		Noam Rodrik
 * @description Main logic of the processor's clock
 */
#include <Core/Clock/Clock.h>
#include <thread>

namespace Core
{
void Clock::SyncClock()
{
	// Sleep until one tick has passed.
	std::this_thread::sleep_until(Clock::GetInstance()._last_tick);

	// Use time_point_cast to convert (via truncation towards zero) back to
	// the "native" duration of high_resolution_clock
	Clock::GetInstance()._last_tick =
		std::chrono::time_point_cast<clock::duration>(Clock::GetInstance()._last_tick + one_clock_period);
}

void Clock::SyncGPUFrame()
{
	const time_point_t CURRENT{std::chrono::high_resolution_clock::now()};

	const time_duration_t SLEEP_LEFT_BETWEEN_FRAMES{
			std::max<float>(DELAYED_TIME - std::chrono::duration_cast<time_duration_t>(CURRENT - Clock::GetInstance()._previous).count(), 0)};

	std::this_thread::sleep_for(SLEEP_LEFT_BETWEEN_FRAMES);

	Clock::GetInstance()._previous = CURRENT;
}
} // Core