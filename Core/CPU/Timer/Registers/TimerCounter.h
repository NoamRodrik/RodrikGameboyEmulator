/**
 * @file		TimerCounter.h
 * @author		Noam Rodrik
 * @description LR35902 main timer counter register definition header.
 */
#ifndef __LR35902_TIMER_COUNTER_REGISTER_H__
#define __LR35902_TIMER_COUNTER_REGISTER_H__

#include <Core/Registers/Memory/MemoryRegister.h>
#include <Core/CPU/Timer/Timer.h>

namespace Core
{
class TimerCounter : public MemoryRegister<API::data_t>
{
public:
	TimerCounter() : MemoryRegister{API::TIMER_COUNTER_ADDRESS} {}
	TimerCounter(const API::data_t value) : MemoryRegister{API::TIMER_COUNTER_ADDRESS} { *this = value; }

public:
	using MemoryRegister::operator API::data_t;
	using MemoryRegister::operator=;

public:
	static constexpr auto TIMER_COUNTER_DEFAULT_VALUE = 0;
};
} // Core

#endif // __LR35902_TIMER_COUNTER_REGISTER_H__