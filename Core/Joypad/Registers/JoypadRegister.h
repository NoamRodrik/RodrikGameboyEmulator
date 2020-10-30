/**
 * @file		JoypadRegister.h
 * @author		Noam Rodrik
 * @description LR35902 main joypad register definition header.
 */
#ifndef __LR35902_JOYPAD_REGISTER_H__
#define __LR35902_JOYPAD_REGISTER_H__

#include <Core/CPU/Registers/DeviceRegister.h>

namespace Core
{
class [[nodiscard]] JoypadRegister : public DeviceRegister<0xFF00>
{
public:
	using DeviceRegister::DeviceRegister;
	using DeviceRegister::operator=;
	using DeviceRegister::operator API::data_t;
};
} // Core

#endif // __LR35902_JOYPAD_REGISTER_H__