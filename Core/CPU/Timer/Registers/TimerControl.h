/**
 * @file		TimerControl.h
 * @author		Noam Rodrik
 * @description LR35902 main timer control register definition header.
 */
#ifndef __LR35902_TIMER_CONTROL_REGISTER_H__
#define __LR35902_TIMER_CONTROL_REGISTER_H__

#include <Core/Registers/Memory/MemoryRegister.h>

namespace Core
{
class TimerControl : public MemoryRegister<API::data_t>
{
public:
	TimerControl() : MemoryRegister{API::TIMER_CONTROL_ADDRESS} {}
	TimerControl(const API::data_t value) : MemoryRegister{API::TIMER_CONTROL_ADDRESS} { *this = value; }

public:
	using MemoryRegister::operator=;
	using MemoryRegister::operator API::data_t;

public:
	static constexpr auto TIMER_CONTROL_DEFAULT_VALUE = 0;
};
} // Core

#endif // __LR35902_TIMER_CONTROL_REGISTER_H__