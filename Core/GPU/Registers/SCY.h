/**
 * @file		SCY.h
 * @author		Noam Rodrik
 * @description LR35902's main gpu scy definition header.
 */
#ifndef __LR35902_SCY_H__
#define __LR35902_SCY_H__

#include <Core/CPU/Registers/DeviceRegister.h>

namespace Core
{
class [[nodiscard]] SCY : public DeviceRegister<0xFF42>
{
public:
	using DeviceRegister::DeviceRegister;
	using DeviceRegister::operator=;
	using DeviceRegister::operator API::data_t;

public:
	static constexpr API::data_t SCY_DEFAULT_VALUE{0x00};
};
} // Core

#endif // __LR35902_SCY_H__