#include "Timer.h"

#include <Core/CPU/Interrupts/InterruptHandler.h>
#include "Registers/TimerControl.h"
#include "Registers/TimerCounter.h"
#include "Registers/TimerModulo.h"
#include "Registers/DividerRegister.h"
#include <Core/Bus/Devices/IORAM.h>

using namespace API;

namespace Core
{
size_t Timer::Clock()
{
	return Timer::IsTimerDividerElapsed() ? Timer::IncreaseDivider() : 0 +
		   Timer::IsTimerCounterElapsed() ? Timer::IncreaseCounter() : 0;
}
	
void Timer::Tick()
{
	Timer::GetInstance().m_divider_tick_amount += 1;
	Timer::GetInstance().m_counter_tick_amount += 1;
}

bool Timer::IsTimerDividerElapsed()
{
	// If we passed the amount of divider ticks, we can clock the divider.
	if (Timer::GetInstance().m_divider_tick_amount % LR35902_HZ_DIVIDER_CLOCK == 0)
	{
		Timer::GetInstance().m_divider_tick_amount = 0;
		return true;
	}

	return false;
}

bool Timer::IsTimerCounterElapsed()
{
	// If we passed the amount of counter ticks, we can clock the counter.
	if (Timer::GetInstance().m_counter_tick_amount % Timer::TimerControlThreshold() == 0)
	{
		Timer::GetInstance().m_counter_tick_amount = 0;
		return true;
	}

	return false;
}

size_t Timer::IncreaseDivider(const data_t amount)
{
	DividerRegister divider{};

	// We need to write to register with force - otherwise it will get reset. (This is the OFFICIAL way to change the counter)
	static gsl::not_null<IORAM*> io_ram = static_cast<IORAM*>(Processor::GetInstance().GetMemory().m_device_manager.GetDeviceAtAddress(DividerRegister::DIVIDER_REGISTER_ADDRESS));
	io_ram->m_memory[io_ram->GetFixedAddress(DividerRegister::DIVIDER_REGISTER_ADDRESS)] += amount;

	if (divider >= DIVIDER_THRESHOLD)
	{
		io_ram->m_memory[io_ram->GetFixedAddress(DividerRegister::DIVIDER_REGISTER_ADDRESS)] = divider - DIVIDER_THRESHOLD;
	}

	// Takes 1 cycle.
	return 1;
}

size_t Timer::IncreaseCounter(const data_t amount)
{
	if (Timer::IsTimerEnabled())
	{
		TimerCounter timer_counter{};
		timer_counter = timer_counter + amount;

		// Takes 1 cycle.
		return 1;
	}

	return 0;
}

bool Timer::IsCounterOverflow(const data_t new_timer_counter)
{
	TimerCounter old_timer_counter{};
	return old_timer_counter == 0xFF && new_timer_counter == 0;
}

void Timer::CounterOverflowInterrupt()
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
	const TimerModulo timer_modulo{};
	TimerCounter timer_counter{timer_modulo};
}

size_t Timer::TimerControlThreshold()
{
	TimerControl timer_control{};

	// Timer clock select
	switch(timer_control & 0x03)
	{
	    case (0): /* 4.096 KHz (1024 cycles) */
		{
			return (1u << 10);
		}
		
	    case (1): /* 262.144 KHz (16 cycles) */
		{
		    return (1u << 4);
		}
	       
	    case (2): /* 65.536 KHz (64 cycles) */
	    {
			return (1u << 6);
		}
		
	    case (3): /* 16.384 KHz (256 cycles) */
		{
			return (1u << 8);
		}

		default:
		{
			STOP_RUNNING("Fetched an invalid value from the timer control!");
		}
	}
}
} // Core