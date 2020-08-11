/**
 * @file		Clock.cpp
 * @author		Noam Rodrik
 * @description Main logic of the processor's clock
 */
#include <Core/Processor/Clock/Clock.h>
#include <Core/Processor/Timer/Timer.h>

namespace Core
{
bool Clock::IsTimerDividerElapsed()
{
	// If we passed the amount of divider ticks, we can clock the divider.
	if (Clock::GetInstance().m_divider_tick_amount % Clock::LR35902_HZ_DIVIDER_CLOCK == 0)
	{
		Clock::GetInstance().m_divider_tick_amount = 0;
		return true;
	}

	return false;
}

bool Clock::IsTimerCounterElapsed()
{
	// If we passed the amount of divider ticks, we can clock the divider.
	if (Clock::GetInstance().m_counter_tick_amount % Timer::TimerControlThreshold() == 0)
	{
		Clock::GetInstance().m_counter_tick_amount = 0;
		return true;
	}

	return false;
}

void Clock::SyncClock()
{
	static constexpr auto one_clock_period{lr35902_clock_period{1}};

	// Find out if the difference is less than one clock period
	while ((std::chrono::high_resolution_clock::now() - Clock::GetInstance().m_last_tick) < one_clock_period);

	// Use time_point_cast to convert (via truncation towards zero) back to
	// the "native" duration of high_resolution_clock
	Clock::GetInstance().m_last_tick =
		std::chrono::time_point_cast<std::chrono::high_resolution_clock::duration>(
			Clock::GetInstance().m_last_tick + one_clock_period);

	Clock::GetInstance().m_divider_tick_amount += 1;
	Clock::GetInstance().m_counter_tick_amount += 1;
}
} // Core