/**
 * @file		Timer.h
 * @author		Noam Rodrik
 * @description LR35902's main timer definition header.
 */
#ifndef __TIMER_H__
#define __TIMER_H__

#include <API/StaticInstance.h>
#include <API/Definitions.h>
#include <cstdint>

namespace Core
{
class Timer : public API::StaticInstance<Timer>
{
public:
	static void Tick();

	// Returns the amount of cycles the operation took.
	static size_t Clock();
	
	static bool IsTimerDividerElapsed();
	static bool IsTimerCounterElapsed();
	static void AssignCounterToModulo();
	static bool IsCounterOverflow(const API::data_t new_timer_counter);
	static size_t TimerControlThreshold();
	static void CounterOverflowInterrupt();
	static size_t IncreaseDivider(const API::data_t amount = 1);
	static size_t IncreaseCounter(const API::data_t amount = 1);
	static bool IsTimerEnabled();

private:
	static constexpr auto DIVIDER_THRESHOLD = 16;

private:
	size_t m_divider_tick_amount{0};
	size_t m_counter_tick_amount{0};
};
} // Core

#endif // __TIMER_H__