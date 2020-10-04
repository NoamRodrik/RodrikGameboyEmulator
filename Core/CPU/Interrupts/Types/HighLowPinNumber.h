/**
 * @file		HighLowPinNumber.h
 * @author		Noam Rodrik
 * @description LR35902 main Transition from High to Low of Pin number P10-P13 interrupt definition header.
 */
#ifndef __HIGH_LOW_PIN_NUMBER_INTERRUPT_H__
#define __HIGH_LOW_PIN_NUMBER_INTERRUPT_H__

#include <Tools/Tools.h>

namespace Core
{
inline const auto HIGH_LOW_PIN_NUMBER_INTERRUPT = []()
{
	Processor::ClearStop();
	return true;
};
} // Core

#endif // __HIGH_LOW_PIN_NUMBER_INTERRUPT_H__