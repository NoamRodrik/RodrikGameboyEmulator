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
private:
	class TimerObject
	{
	public:
		size_t GetTime() const;
		void Increase(size_t amount);
		void Lower(size_t amount);

	private:
		size_t _time{0};
	};
public:
	static void Clock(size_t cycles);
	static bool IsCounterOverflow(const API::data_t new_timer_counter);
	static void LaunchInterrupt();

private:
	static void Tick();
	static size_t TimerControlThreshold();
	static bool IsTimerEnabled();
	static void AssignCounterToModulo();
	static bool MachinePassedThreshold();
	static bool CounterPassedThreshold();
	static bool DividerPassedThreshold();

private:
	static TimerObject& Machine();
	static TimerObject& Counter();
	static TimerObject& Divider();

private:
	static constexpr auto TIMER_THRESHOLD{16};

private:
	TimerObject _machine{};
	TimerObject _counter{};
	TimerObject _divider{};
};
} // Core

#endif // __TIMER_H__