/**
 * @file		Clock.h
 * @author		Noam Rodrik
 * @description LR35902's main clock definition header.
 */
#ifndef __CYCLE_H__
#define __CYCLE_H__

#include <API/StaticInstance.h>
#include <API/Definitions.h>
#include <chrono>

using namespace std::chrono_literals;

namespace Core
{
class [[nodiscard]] Clock : public API::StaticInstance<Clock>
{
public:
	Clock() = default;
	~Clock() = default;

public:
	static void SyncClock();
	static void SyncGPUFrame();

private:
	using lr35902_clock_period = std::chrono::duration<int64_t, std::ratio<1, API::LR35902_HZ_CLOCK>>;
	static constexpr lr35902_clock_period one_clock_period{1};
	using clock = std::chrono::high_resolution_clock;

private:
	using time_point_t = std::chrono::time_point<std::chrono::high_resolution_clock>;
	using time_duration_t = std::chrono::duration<float, std::milli>;

private:
	static constexpr float FRAMES_PER_SECOND{59.7f};
	static constexpr float DELAYED_TIME{1000.0f / FRAMES_PER_SECOND};

private:
	time_point_t		   _previous{std::chrono::high_resolution_clock::now()};
	decltype(clock::now()) _last_tick{std::chrono::time_point_cast<clock::duration>(clock::now() + one_clock_period)};
};
} // Core

#endif // __PROCESSOR_H__
