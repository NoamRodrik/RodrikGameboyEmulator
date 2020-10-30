/**
 * @file		WY.h
 * @author		Noam Rodrik
 * @description LR35902's main gpu wy definition header.
 */
#ifndef __LR35902_WY_H__
#define __LR35902_WY_H__

#include <Core/CPU/Registers/DeviceRegister.h>

namespace Core
{
class [[nodiscard]] WY : public DeviceRegister<0xFF4A>
{
public:
	using DeviceRegister::DeviceRegister;
	using DeviceRegister::operator=;
	using DeviceRegister::operator API::data_t;

public:
	static constexpr API::data_t WY_DEFAULT_VALUE{0x00};
};
} // Core

#endif // __LR35902_WY_H__