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
	// Returns the amount of cycles the operation took.
	static void Clock(size_t cycles);
	static void AssignCounterToModulo();
	static bool IsCounterOverflow(const API::data_t new_timer_counter);

private:
	static size_t TimerControlThreshold();
	static bool IsTimerEnabled();
	static void HandleInterruptOnOverflow();
	static void IncreaseDivider(const API::data_t cycles = 1);
	static void IncreaseCounter(const API::data_t cycles = 1);
	static bool TimerCounterStarted();
	static void ResetCounterStarted();
	static void SetCounterStarted();
	static bool OverflowOccurred();
	static void ResetOverflowOccurred();
	static void SetOverflowOccurred();
	static bool IsTimerElapsed();
	static void SetTimerElapsed(bool condition);

private:
	bool m_counter_started{false};
	bool m_save_timer_elapse{false};
	bool m_overflow_occurred{false};
};
} // Core

#endif // __TIMER_H__