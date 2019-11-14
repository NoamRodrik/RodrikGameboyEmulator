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
	/* Returns true if timer has overflown. */
	inline static void IncrementCycles() { Timer::GetInstance().m_counter_cycles += 1;  Timer::GetInstance().m_divider_cycles += 1; }
	static void IncrementCounter();
	static void UpdateTimerControl(const data_t data);
	static size_t CountChange(const bool set_divider = true);

	static void AssignCounterToModulo();
	static void AssignDividerToZero();
	inline static bool IsTimerLoading() { return Timer::GetInstance().m_timer_loading; }
	inline static void SetLoading() { Timer::GetInstance().m_timer_loading = true; }
	inline static void ClearLoading() { Timer::GetInstance().m_timer_loading = false; }
	inline static bool IsTimerOverflowing() { return Timer::GetInstance().m_timer_overflow; }
	inline static void SetOverflowing() { Timer::GetInstance().m_timer_overflow = true; }
	inline static void ClearOverflowing() { Timer::GetInstance().m_timer_overflow = false; }
	static bool TimerControlBit(const data_t timer_control, const address_t cycles);

private:
	address_t m_counter_cycles{0};
	address_t m_divider_cycles{0};
	bool	 m_timer_loading{false};
	bool	 m_timer_overflow{false};
};
} // Core

#endif // __TIMER_H__