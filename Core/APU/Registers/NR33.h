/**
 * @file		NR33.h
 * @author		Noam Rodrik
 * @description LR35902's main apu NR33 definition header.
 */
#ifndef __LR35902_NR33_H__
#define __LR35902_NR33_H__

#include <Core/CPU/Registers/DeviceRegister.h>

namespace Core
{
class [[nodiscard]] NR33 : public DeviceRegister<0xFF1D>
{
public:
	using DeviceRegister::DeviceRegister;
	using DeviceRegister::operator=;
	using DeviceRegister::operator API::data_t;

public:
	static constexpr API::data_t NR33_DEFAULT_VALUE{0x00};
};
} // Core

#endif // __LR35902_NR33_H__