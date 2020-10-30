/**
 * @file		SCX.h
 * @author		Noam Rodrik
 * @description LR35902's main gpu scx definition header.
 */
#ifndef __LR35902_SCX_H__
#define __LR35902_SCX_H__

#include <Core/CPU/Registers/DeviceRegister.h>

namespace Core
{
class [[nodiscard]] SCX : public DeviceRegister<0xFF43>
{
public:
	using DeviceRegister::DeviceRegister;
	using DeviceRegister::operator=;
	using DeviceRegister::operator API::data_t;

public:
	static constexpr API::data_t SCX_DEFAULT_VALUE{0x00};
};
} // Core

#endif // __LR35902_SCX_H__