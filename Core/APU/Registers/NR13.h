/**
 * @file		NR13.h
 * @author		Noam Rodrik
 * @description LR35902's main apu NR13 definition header.
 */
#ifndef __LR35902_NR13_H__
#define __LR35902_NR13_H__

#include <Core/CPU/Registers/DeviceRegister.h>

namespace Core
{
class [[nodiscard]] NR13 : public DeviceRegister<0xFF13>
{
public:
	using DeviceRegister::DeviceRegister;
	using DeviceRegister::operator=;
	using DeviceRegister::operator API::data_t;

public:
	static constexpr API::data_t NR13_DEFAULT_VALUE{0x00};
};
} // Core

#endif // __LR35902_NR13_H__