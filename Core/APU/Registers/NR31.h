/**
 * @file		NR31.h
 * @author		Noam Rodrik
 * @description LR35902's main apu NR31 definition header.
 */
#ifndef __LR35902_NR31_H__
#define __LR35902_NR31_H__

#include <Core/CPU/Registers/DeviceRegister.h>

namespace Core
{
class [[nodiscard]] NR31 : public DeviceRegister<0xFF1B>
{
public:
	using DeviceRegister::DeviceRegister;
	using DeviceRegister::operator=;
	using DeviceRegister::operator API::data_t;

public:
	static constexpr API::data_t NR31_DEFAULT_VALUE{0xFF};
};
} // Core

#endif // __LR35902_NR31_H__