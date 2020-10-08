/**
 * @file		JoypadRegister.h
 * @author		Noam Rodrik
 * @description LR35902 main joypad register definition header.
 */
#ifndef __LR35902_JOYPAD_REGISTER_H__
#define __LR35902_JOYPAD_REGISTER_H__

#include <Core/CPU/Registers/MemoryRegister.h>

namespace Core
{
class [[nodiscard]] JoypadRegister : public MemoryRegister<API::data_t>
{
public:
	JoypadRegister() : MemoryRegister{JOYPAD_REGISTER_ADDRESS} {}
	JoypadRegister(const API::data_t value) : MemoryRegister{JOYPAD_REGISTER_ADDRESS} { *this = value; }

public:
	using MemoryRegister::operator=;
	using MemoryRegister::operator API::data_t;

public:
	static constexpr API::address_t JOYPAD_REGISTER_ADDRESS{0xFF00};
};
} // Core

#endif // __LR35902_JOYPAD_REGISTER_H__