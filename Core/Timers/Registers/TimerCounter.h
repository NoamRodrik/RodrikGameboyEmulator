/**
 * @file		TimerCounter.h
 * @author		Noam Rodrik
 * @description LR35902 main timer counter register definition header.
 */
#ifndef __LR35902_TIMER_COUNTER_REGISTER_H__
#define __LR35902_TIMER_COUNTER_REGISTER_H__

#include <Core/CPU/Registers/DeviceRegister.h>

namespace Core
{
class [[nodiscard]] TimerCounter : public DeviceRegister<0xFF05>
{
public:
	using DeviceRegister::DeviceRegister;
	using DeviceRegister::operator=;
	using DeviceRegister::operator API::data_t;

public:
	static constexpr API::data_t TIMER_COUNTER_DEFAULT_VALUE{0x00};
};
} // Core

#endif // __LR35902_TIMER_COUNTER_REGISTER_H__