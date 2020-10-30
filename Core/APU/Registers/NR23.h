/**
 * @file		NR23.h
 * @author		Noam Rodrik
 * @description LR35902's main apu NR23 definition header.
 */
#ifndef __LR35902_NR23_H__
#define __LR35902_NR23_H__

#include <Core/CPU/Registers/DeviceRegister.h>

namespace Core
{
class [[nodiscard]] NR23 : public DeviceRegister<0xFF18>
{
public:
	using DeviceRegister::DeviceRegister;
	using DeviceRegister::operator=;
	using DeviceRegister::operator API::data_t;

public:
	static constexpr API::data_t NR23_DEFAULT_VALUE{0x00};
};
} // Core

#endif // __LR35902_NR23_H__