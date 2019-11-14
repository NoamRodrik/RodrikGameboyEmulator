#include "Timer.h"

#include <Core/Interrupts/InterruptHandler.h>
#include "Registers/TimerControl.h"
#include "Registers/TimerCounter.h"
#include "Registers/TimerModulo.h"
#include "Registers/DividerRegister.h"

namespace Core
{
void Timer::IncrementCounter()
{
	TimerCounter timer{};
	Timer::GetInstance().m_timer_overflow = (timer == 0xFF);
	timer = timer + 1;
}

size_t Timer::CountChange(bool set_divider)
{
	const address_t NEW_COUNT = set_divider ? 0 : Timer::GetInstance().m_divider_cycles + 4;
	const TimerControl timer_control{};
	if (timer_control & 0x04)
	{
		if (!Timer::GetInstance().TimerControlBit(timer_control, NEW_COUNT) &&
			 Timer::GetInstance().TimerControlBit(timer_control, Timer::GetInstance().m_divider_cycles))
		{
			Timer::GetInstance().IncrementCounter();
		}
	}

	Timer::GetInstance().m_divider_cycles = NEW_COUNT;

	if (set_divider)
	{
		DividerRegister divider{static_cast<const data_t>(NEW_COUNT >> 8)};
	}

	// Takes 4 cycles.
	return 4;
}
	
void Timer::UpdateTimerControl(const data_t data)
{
	const data_t old_data = TimerControl{};

	/* When disabled the bit to the falling edge detector is zero */
	const bool oldBit = (old_data & 0x04) && Timer::GetInstance().TimerControlBit(old_data, Timer::GetInstance().m_counter_cycles);
	const bool newBit = (data & 0x04) && Timer::GetInstance().TimerControlBit(data, Timer::GetInstance().m_counter_cycles);

	/* Check for falling edge */
	if (oldBit && !newBit)
	{
		Timer::GetInstance().IncrementCounter();
	}
}

void Timer::AssignCounterToModulo()
{
	const TimerModulo timer_modulo{};
	TimerCounter timer_counter{timer_modulo};
}

void Timer::AssignDividerToZero()
{
	DividerRegister divider{0};
}

bool Timer::TimerControlBit(const data_t timer_control, const address_t cycles)
{
	// Timer clock select
	switch(timer_control & 0x03)
	{
	    case (0): /* 4.096 KHz (1024 cycles) */
		{
			return cycles & (1u << 9);
		}
		
	    case (1): /* 262.144 KHz (16 cycles) */
		{
		    return cycles & (1u << 3);
		}
	       
	    case (2): /* 65.536 KHz (64 cycles) */
	    {
			return cycles & (1u << 5);
		}
		
	    case (3): /* 16.384 KHz (256 cycles) */
		{
			return cycles & (1u << 7);
		}

		default:
		{
			STOP_RUNNING("Fetched an invalid value from the timer control!");
		}
	}
}
} // Core