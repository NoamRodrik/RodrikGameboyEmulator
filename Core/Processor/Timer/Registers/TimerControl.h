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
class TimerControl : public MemoryRegister<data_t>
{
public:
	TimerControl() : MemoryRegister{TIMER_CONTROL_ADDRESS} {}
	TimerControl(const data_t value) : MemoryRegister{TIMER_CONTROL_ADDRESS} { *this = value; }

public:
	using MemoryRegister::operator=;
	using MemoryRegister::operator data_t;

private:
	static constexpr address_t TIMER_CONTROL_ADDRESS{0xFF07};
};
} // Core

#endif // __LR35902_TIMER_CONTROL_REGISTER_H__