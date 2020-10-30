/**
 * @file		WX.h
 * @author		Noam Rodrik
 * @description LR35902's main gpu wx definition header.
 */
#ifndef __LR35902_WX_H__
#define __LR35902_WX_H__

#include <Core/CPU/Registers/DeviceRegister.h>

namespace Core
{
class [[nodiscard]] WX : public DeviceRegister<0xFF4B>
{
public:
	using DeviceRegister::DeviceRegister;
	using DeviceRegister::operator=;
	using DeviceRegister::operator API::data_t;

public:
	static constexpr API::data_t WX_DEFAULT_VALUE{0x00};
};
} // Core

#endif // __LR35902_WX_H__