/**
 * @file		Clock.h
 * @author		Noam Rodrik
 * @description LR35902's main clock definition header.
 */
#ifndef __CYCLE_H__
#define __CYCLE_H__

#include <Core/API/StaticInstance.h>
#include <chrono>

namespace Core
{
using namespace std::chrono_literals;

class Clock : public StaticInstance<Clock>
{
public:
	Clock() = default;
	~Clock() = default;

public:
	static void SyncClock();

	static bool IsTimerDividerElapsed();
	static bool IsTimerCounterElapsed();

private:
	static constexpr auto LR35902_HZ_CLOCK = 1'048'576;
	static constexpr auto LR35902_HZ_DIVIDER_CLOCK = 4'096;
	using lr35902_clock_period = std::chrono::duration<long long, std::ratio<1, LR35902_HZ_CLOCK>>;
	using divider_clock_period = std::chrono::duration<long long, std::ratio<1, LR35902_HZ_DIVIDER_CLOCK>>;

private:
	decltype(std::chrono::high_resolution_clock::now()) m_last_tick{};
	size_t                                              m_divider_tick_amount{0};
	size_t                                              m_counter_tick_amount{0};
};
} // Core

#endif // __PROCESSOR_H__
