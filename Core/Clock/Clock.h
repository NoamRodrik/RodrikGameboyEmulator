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
	static void Update();

private:
	using lr35902_clock_period = std::chrono::duration<long long, std::ratio<1, API::LR35902_HZ_CLOCK>>;

private:
	decltype(std::chrono::high_resolution_clock::now()) m_last_tick{};
};
} // Core

#endif // __PROCESSOR_H__
