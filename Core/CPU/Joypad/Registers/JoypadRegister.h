/**
 * @file		JoypadRegister.h
 * @author		Noam Rodrik
 * @description LR35902 main joypad register definition header.
 */
#ifndef __LR35902_JOYPAD_REGISTER_H__
#define __LR35902_JOYPAD_REGISTER_H__

#include <Core/Registers/Memory/MemoryRegister.h>

namespace Core
{
class JoypadRegister : public MemoryRegister<data_t>
{
public:
	JoypadRegister() : MemoryRegister{JOYPAD_REGISTER_ADDRESS} {}
	JoypadRegister(const data_t value) : MemoryRegister{JOYPAD_REGISTER_ADDRESS} { *this = value; }

public:
	using MemoryRegister::operator=;
	using MemoryRegister::operator data_t;
};
} // Core

#endif // __LR35902_JOYPAD_REGISTER_H__