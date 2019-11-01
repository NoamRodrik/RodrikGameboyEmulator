/**
 * @file		Timer.h
 * @author		Noam Rodrik
 * @description LR35902's main timer definition header.
 */
#ifndef __TIMER_H__
#define __TIMER_H__

#include <Core/API/StaticInstance.h>
#include <cstdint>

namespace Core
{
class Timer : public StaticInstance<Timer>
{
public:
	/* Returns true if timer has overflown. */
	static bool Increment();

	static void AssignCounterToModulo();
	inline static bool IsTimerLoading() { return Timer::GetInstance().m_timer_loading; }
	inline static void SetLoading() { Timer::GetInstance().m_timer_loading = true; }
	inline static void ClearLoading() { Timer::GetInstance().m_timer_loading = false; }

private:
	static bool TimerControlBit();

private:
	uint16_t m_cycles{0};
	bool	 m_timer_loading{false};
};
} // Core

#endif // __TIMER_H__