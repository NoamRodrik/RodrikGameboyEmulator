/**
 * @file		Clock.cpp
 * @author		Noam Rodrik
 * @description Main logic of the processor's clock
 */
#include <Core/CPU/Processor.h>
#include <Core/Clock/Clock.h>
#include <thread>

namespace Core
{
void Clock::SyncClock()
{
#ifndef DEBUG_COMPILATION
	// Sleep until one tick has passed.
	std::this_thread::sleep_until(Clock::GetInstance()._last_tick);
#endif

	// Use time_point_cast to convert (via truncation towards zero) back to
	// the "native" duration of high_resolution_clock
	Clock::GetInstance()._last_tick =
		std::chrono::time_point_cast<clock::duration>(Clock::GetInstance()._last_tick + one_clock_period);
}

void Clock::SyncGPUFrame()
{
	const float targetFPS = 59.7f;
	const std::chrono::duration<float> targetFrameDuration(1.0f / targetFPS);

#ifndef DEBUG_COMPILATION
	std::this_thread::sleep_for(targetFrameDuration);
#endif
}
} // Core