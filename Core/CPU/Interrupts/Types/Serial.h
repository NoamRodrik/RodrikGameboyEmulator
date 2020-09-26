/**
 * @file		Serial.h
 * @author		Noam Rodrik
 * @description LR35902 main Serial interrupt definition header.
 */
#ifndef __SERIAL_INTERRUPT_H__
#define __SERIAL_INTERRUPT_H__

#include <Tools/Tools.h>

namespace Core
{
inline const auto SERIAL_INTERRUPT = []()
{
	LOG("Not implementing serial interrupts in this emulator yet!");
	return true;
};
} // Core

#endif // __SERIAL_INTERRUPT_H__