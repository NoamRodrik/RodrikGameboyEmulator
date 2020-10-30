/**
 * @file		TimerControl.h
 * @author		Noam Rodrik
 * @description LR35902 main timer control register definition header.
 */
#ifndef __LR35902_TIMER_CONTROL_REGISTER_H__
#define __LR35902_TIMER_CONTROL_REGISTER_H__

#include <Core/CPU/Registers/DeviceRegister.h>

namespace Core
{
class [[nodiscard]] TimerControl : public DeviceRegister<0xFF07>
{
public:
	using DeviceRegister::DeviceRegister;
	using DeviceRegister::operator=;
	using DeviceRegister::operator API::data_t;

public:
	static constexpr API::data_t TIMER_CONTROL_DEFAULT_VALUE{0x00};
};
} // Core

#endif // __LR35902_TIMER_CONTROL_REGISTER_H__