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
	const bool timer_overflow = (timer == 0xFF);
	timer = timer + 1;
	Timer::GetInstance().m_timer_overflow = timer_overflow;
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

size_t Timer::IncrementDivider()
{
	size_t clock_cycles = 0;
	
	// The divider timer updates every 256.
	if (Timer::GetInstance().m_divider_cycles > 256)
	{
		Timer::GetInstance().m_divider_cycles -= 256;

		const TimerControl timer_control{};

		// If the timer is enabled.
		if (timer_control & 0x04)
		{
			DividerRegister timer{};

			// Check if there's an overflow.
			if (!TimerControlBit(timer_control, 0) &&
				 TimerControlBit(timer_control, Timer::GetInstance().m_counter_cycles))
			{
				Timer::GetInstance().IncrementCounter();
			}
			
			timer = timer + 1;

			// Every timer increment takes 4 cycles.
			clock_cycles += 4;
		}

		return clock_cycles;
	}

	return 0;
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