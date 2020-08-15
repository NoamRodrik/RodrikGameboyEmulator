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
void Timer::Clock(size_t cycles)
{
	// Order here is important!
	Timer::IncreaseDivider(static_cast<const data_t>(cycles));
	Timer::IncreaseCounter(static_cast<const data_t>(cycles));
}

void Timer::IncreaseDivider(const data_t cycles)
{
	address_t full_divider = static_cast<data_t>(DividerRegister{}) << 8 | READ_DATA_AT(DividerRegister::DIVIDER_REGISTER_ADDRESS_LSB);
	full_divider += cycles;

	// We need to write to register with force - otherwise it will get reset. (This is the OFFICIAL way to change the counter)
	static gsl::not_null<IORAM*> io_ram{static_cast<IORAM*>(Processor::GetInstance().GetMemory().m_device_manager.GetDeviceAtAddress(DividerRegister::DIVIDER_REGISTER_ADDRESS))};
	io_ram->m_memory[io_ram->GetFixedAddress(DividerRegister::DIVIDER_REGISTER_ADDRESS)] = (full_divider >> 8) & 0x00FF;
	io_ram->m_memory[io_ram->GetFixedAddress(DividerRegister::DIVIDER_REGISTER_ADDRESS_LSB)] = full_divider & 0x00FF;
}

void Timer::IncreaseCounter(const data_t cycles)
{
	if (Timer::IsTimerEnabled())
	{
		// Fetching the internal clock
		const address_t full_divider = static_cast<data_t>(DividerRegister{}) << 8 | READ_DATA_AT(DividerRegister::DIVIDER_REGISTER_ADDRESS_LSB);

		// If the timer just started, we will save the clock count at start
		// This way we don't increment the timer to early, because the internal
		// clock may be not zero at this time.
		const address_t previous_full_divider = Timer::TimerCounterStarted() ? full_divider - 1 : full_divider - cycles;
		Timer::SetCounterStarted();

		const size_t clock_bit{(API::LR35902_HZ_CLOCK / Timer::TimerControlThreshold()) >> 1};
		for (size_t clock = static_cast<size_t>(previous_full_divider) + 1; clock <= full_divider; ++clock)
		{
			Timer::HandleInterruptOnOverflow();
			const bool TIMER_ELAPSE_CHECK = ((clock & clock_bit) != 0);

			if (Timer::IsTimerElapsed() && !TIMER_ELAPSE_CHECK)
			{
				TimerCounter timer_counter{};

				// Overflow
				if (Timer::IsCounterOverflow(static_cast<data_t>(timer_counter) + 1))
				{
					Timer::SetOverflowOccurred();
				}

				timer_counter = timer_counter + 1;
			}

			Timer::SetTimerElapsed(TIMER_ELAPSE_CHECK);
		}
	}
	else
	{
		Timer::ResetCounterStarted();
	}
}

bool Timer::IsCounterOverflow(const data_t new_timer_counter)
{
	TimerCounter old_timer_counter{};
	return old_timer_counter == 0xFF && new_timer_counter == 0;
}

void Timer::HandleInterruptOnOverflow()
{
	if (Timer::OverflowOccurred())
	{
		Timer::ResetOverflowOccurred();
		InterruptHandler::IRQ(EInterrupts::TIMER);
	}
}

bool Timer::IsTimerEnabled()
{
	const TimerControl timer_control{};
	return timer_control & 0b00000100;
}

bool Timer::TimerCounterStarted()
{
	return Timer::GetInstance().m_counter_started;
}

void Timer::SetCounterStarted()
{
	Timer::GetInstance().m_counter_started = true;
}

void Timer::ResetCounterStarted()
{
	Timer::GetInstance().m_counter_started = false;
}

bool Timer::OverflowOccurred()
{
	return Timer::GetInstance().m_overflow_occurred;
}

void Timer::SetOverflowOccurred()
{
	Timer::GetInstance().m_overflow_occurred = true;
}

void Timer::ResetOverflowOccurred()
{
	Timer::GetInstance().m_overflow_occurred = false;
}

bool Timer::IsTimerElapsed()
{
	return Timer::GetInstance().m_save_timer_elapse;
}

void Timer::SetTimerElapsed(bool condition)
{
	Timer::GetInstance().m_save_timer_elapse = condition;
}

void Timer::AssignCounterToModulo()
{
	TimerCounter timer_counter{TimerModulo{}};
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