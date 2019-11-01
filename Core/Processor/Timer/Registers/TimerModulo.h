/**
 * @file		TimerModulo.h
 * @author		Noam Rodrik
 * @description LR35902 main timer modulo register definition header.
 */
#ifndef __LR35902_TIMER_MODULO_REGISTER_H__
#define __LR35902_TIMER_MODULO_REGISTER_H__

#include <Core/Registers/Memory/MemoryRegister.h>
#include "TimerCounter.h"

namespace Core
{
class TimerModulo : public MemoryRegister<data_t>
{
public:
	TimerModulo() : MemoryRegister{TIMER_MODULO_ADDRESS} {}
	TimerModulo(const data_t value) : MemoryRegister{TIMER_MODULO_ADDRESS}
	{
		*this = value;

		/* Whilst the modulo is being loaded any writes are effective immediately */
		if (Timer::GetInstance().IsTimerLoading())
		{
			TimerCounter timer_counter{value};
		}
	}

public:
	using MemoryRegister::operator data_t;

private:
	using MemoryRegister::operator=;

private:
	static constexpr address_t TIMER_MODULO_ADDRESS{0xFF06};
};
} // Core

#endif // __LR35902_TIMER_MODULO_REGISTER_H__