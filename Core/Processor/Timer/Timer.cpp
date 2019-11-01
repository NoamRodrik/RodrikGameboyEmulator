#include "Timer.h"

#include <Core/Interrupts/InterruptHandler.h>
#include "Registers/TimerControl.h"
#include "Registers/TimerCounter.h"
#include "Registers/TimerModulo.h"
#include "Registers/DividerRegister.h"

namespace Core
{
bool Timer::Increment()
{
	TimerCounter timer{};
	const bool timer_overflow = (timer == 0xFF);
	timer.Increment();
	return timer_overflow;
}

void Timer::AssignCounterToModulo()
{
	const TimerModulo timer_modulo{};
	TimerCounter timer_counter{timer_modulo};
}

bool Timer::TimerControlBit()
{
	const TimerControl timer_control{};
	
	// Timer clock select
	switch(timer_control & 0x03)
	{
    case (0): /* 4.096 KHz (1024 cycles) */
	{
		return timer_control & (1u << 9);
	}
	
    case (1): /* 262.144 KHz (16 cycles) */
	{
	    return timer_control & (1u << 3);
	}
       
    case (2): /* 65.536 KHz (64 cycles) */
    {
		return timer_control & (1u << 5);
	}
	
    case (3): /* 16.384 KHz (256 cycles) */
	{
		return timer_control & (1u << 7);
	}

	default:
	{
		STOP_RUNNING("Fetched an invalid value from the timer control!");
	}
	}
}
} // Core