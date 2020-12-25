/**
 * @file		LY.h
 * @author		Noam Rodrik
 * @description LR35902's main gpu ly definition header.
 */
#ifndef __LR35902_LY_H__
#define __LR35902_LY_H__

#include <Core/CPU/Registers/DeviceRegister.h>

namespace Core
{
/* Read only (for the game!) */
class [[nodiscard]] LY : public DeviceRegister<0xFF44>
{
public:
	using DeviceRegister::DeviceRegister;
	using DeviceRegister::operator=;
	using DeviceRegister::operator API::data_t;

public:
	static constexpr API::data_t LY_DEFAULT_VALUE{0x00};
};
} // Core

#endif // __LR35902_LY_H__