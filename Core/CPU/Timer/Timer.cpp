#include "Timer.h"

#include <Core/CPU/Interrupts/InterruptHandler.h>
#include "Registers/TimerControl.h"
#include "Registers/TimerCounter.h"
#include "Registers/TimerModulo.h"
#include "Registers/DividerRegister.h"
#include <Core/Bus/Devices/IORAM.h>

namespace Core
{
size_t Timer::IncreaseDivider(data_t amount)
{
	DividerRegister divider{};

	// We need to write to register with force - otherwise it will get reset. (This is the OFFICIAL way to change the counter)
	static gsl::not_null<IORAM*> io_ram = static_cast<IORAM*>(Processor::GetInstance().GetMemory().m_device_manager.GetDeviceAtAddress(DIVIDER_REGISTER_ADDRESS));
	io_ram->m_memory[io_ram->GetFixedAddress(DIVIDER_REGISTER_ADDRESS)] += amount;

	if (divider >= DIVIDER_THRESHOLD)
	{
		io_ram->m_memory[io_ram->GetFixedAddress(DIVIDER_REGISTER_ADDRESS)] = divider - DIVIDER_THRESHOLD;
	}

	// Takes 1 cycle.
	return 1;
}

size_t Timer::IncreaseCounter(data_t amount)
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