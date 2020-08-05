/**
 * @file		Timer.h
 * @author		Noam Rodrik
 * @description LR35902's main timer definition header.
 */
#ifndef __TIMER_H__
#define __TIMER_H__

#include <Core/API/StaticInstance.h>
#include <Core/API/Definitions.h>
#include <cstdint>

namespace Core
{
class Timer : public StaticInstance<Timer>
{
public:
	static void AssignCounterToModulo();
	static bool IsCounterOverflow(const data_t new_timer_counter);
	static size_t TimerControlThreshold();
	static void CounterOverflowInterrupt();
	static size_t IncreaseDivider(data_t amount = 1);
	static size_t IncreaseCounter(data_t amount = 1);
	static bool IsTimerEnabled();

private:
	static constexpr auto DIVIDER_THRESHOLD = 64;
};
} // Core

#endif // __TIMER_H__