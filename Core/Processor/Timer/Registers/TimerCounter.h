/**
 * @file		TimerCounter.h
 * @author		Noam Rodrik
 * @description LR35902 main timer counter register definition header.
 */
#ifndef __LR35902_TIMER_COUNTER_REGISTER_H__
#define __LR35902_TIMER_COUNTER_REGISTER_H__

#include <Core/Registers/Memory/MemoryRegister.h>
#include <Core/Processor/Timer/Timer.h>

namespace Core
{
class TimerCounter : public MemoryRegister<data_t>
{
public:
	TimerCounter() : MemoryRegister{TIMER_COUNTER_ADDRESS} {}
	TimerCounter(const data_t value) : MemoryRegister{TIMER_COUNTER_ADDRESS}
	{
		/* Writes to the timer counter whilst it is loading are ignored */
		if (!Timer::GetInstance().IsTimerLoading())
		{
			*this = value;
			/* Writing to timer counter suppresses any pending overflow effects */ // TODO!
		}
	}

	void Increment()
	{
		*this = TimerCounter{} + 1;
	}

public:
	using MemoryRegister::operator data_t;

private:
	using MemoryRegister::operator=;

private:
	static constexpr address_t TIMER_COUNTER_ADDRESS{0xFF05};
};
} // Core

#endif // __LR35902_TIMER_COUNTER_REGISTER_H__