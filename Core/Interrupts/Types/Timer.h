/**
 * @file		Timer.h
 * @author		Noam Rodrik
 * @description LR35902 main Timer interrupt definition header.
 */
#ifndef __TIMER_INTERRUPT_H__
#define __TIMER_INTERRUPT_H__

#include <Tools/Tools.h>

namespace Core
{
inline const auto TIMER_INTERRUPT = []()
{
	LOG("Timer has timed out!");
	return true;
};
} // Core

#endif // __TIMER_INTERRUPT_H__