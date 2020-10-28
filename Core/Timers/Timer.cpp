#include "Timer.h"

#include <Core/CPU/Interrupts/InterruptHandler.h>
#include "Registers/TimerControl.h"
#include "Registers/TimerCounter.h"
#include "Registers/TimerModulo.h"
#include "Registers/DividerRegister.h"
#include <Core/Bus/Devices/IODevice.h>

using namespace API;

namespace Core
{
size_t Timer::TimerObject::GetTime() const
{
	return this->_time;
}

void Timer::TimerObject::Increase(size_t amount)
{
	this->_time += amount;
}

void Timer::TimerObject::Lower(size_t amount)
{
	this->_time -= amount;
}

void Timer::Clock(size_t cycles)
{
	Timer::Machine().Increase(cycles);

	while (Timer::MachinePassedThreshold())
	{
		Timer::Machine().Lower(Timer::TIMER_THRESHOLD);
		Timer::Tick();
	}
}

void Timer::Tick()
{
	TimerCounter timer_counter{};

	Timer::Counter().Increase(1);
	Timer::Divider().Increase(1);

	while (Timer::DividerPassedThreshold())
	{
		Timer::Divider().Lower(Timer::TIMER_THRESHOLD);
		SANITY(Processor::GetInstance().GetMemory().WriteDirectly(DividerRegister::DIVIDER_REGISTER_ADDRESS, DividerRegister{} + 1),
			   "Failed writing the div register value directly");
	}

	if (Timer::IsTimerEnabled())
	{
		while (Timer::CounterPassedThreshold())
		{
			Timer::Counter().Lower(Timer::TimerControlThreshold());
			timer_counter = timer_counter + 1;
		}
	}
}

Timer::TimerObject& Timer::Machine()
{
	return Timer::GetInstance()._machine;
}

Timer::TimerObject& Timer::Counter()
{
	return Timer::GetInstance()._counter;
}

Timer::TimerObject& Timer::Divider()
{
	return Timer::GetInstance()._divider;
}

bool Timer::MachinePassedThreshold()
{
	return Timer::Machine().GetTime() >= Timer::TIMER_THRESHOLD;
}

bool Timer::CounterPassedThreshold()
{
	return Timer::Counter().GetTime() >= Timer::TimerControlThreshold();
}

bool Timer::DividerPassedThreshold()
{
	return Timer::Divider().GetTime() >= Timer::TIMER_THRESHOLD;
}

bool Timer::IsCounterOverflow(const data_t new_timer_counter)
{
	TimerCounter old_timer_counter{};
	return old_timer_counter == 0xFF && new_timer_counter == 0;
}

void Timer::LaunchInterrupt()
{
	InterruptHandler::IRQ(EInterrupts::TIMER);
}

bool Timer::IsTimerEnabled()
{
	const TimerControl timer_control{};
	return timer_control & 0b00000100;
}

void Timer::AssignCounterToModulo()
{
	TimerCounter timer_counter{TimerModulo{}};
}

size_t Timer::TimerControlThreshold()
{
	TimerControl timer_control{};

	// Timer clock select
	switch (timer_control & 0x03)
	{
	    case (0): /* 4.096 KHz (1024 cycles) */
		{
			return (1u << 6);
		}
		
	    case (1): /* 262.144 KHz (16 cycles) */
		{
		    return (1u << 0);
		}
	       
	    case (2): /* 65.536 KHz (64 cycles) */
	    {
			return (1u << 2);
		}
		
	    case (3): /* 16.384 KHz (256 cycles) */
		{
			return (1u << 4);
		}

		default:
		{
			STOP_RUNNING("Fetched an invalid value from the timer control!");
		}
	}
}
} // Core