/**
 * @file		Clock.h
 * @author		Noam Rodrik
 * @description LR35902's main clock definition header.
 */
#ifndef __CYCLE_H__
#define __CYCLE_H__

#include <API/StaticInstance.h>
#include <chrono>

namespace Core
{
using namespace std::chrono_literals;

class Clock : public API::StaticInstance<Clock>
{
public:
	Clock() = default;
	~Clock() = default;

public:
	static void SyncClock();

private:
	using lr35902_clock_period = std::chrono::duration<int64_t, std::ratio<1, API::LR35902_HZ_CLOCK>>;
	static constexpr lr35902_clock_period one_clock_period{1};
	using clock = std::chrono::high_resolution_clock;

private:
	decltype(clock::now()) m_last_tick{std::chrono::time_point_cast<clock::duration>(clock::now() + one_clock_period)};
};
} // Core

#endif // __PROCESSOR_H__
