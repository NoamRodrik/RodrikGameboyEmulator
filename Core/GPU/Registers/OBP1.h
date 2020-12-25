/**
 * @file		OBP1.h
 * @author		Noam Rodrik
 * @description LR35902's main gpu obp1 definition header.
 */
#ifndef __LR35902_OBP1_H__
#define __LR35902_OBP1_H__

#include <Core/CPU/Registers/DeviceRegister.h>

namespace Core
{
class [[nodiscard]] OBP1 : public DeviceRegister<0xFF49>
{
public:
	using DeviceRegister::DeviceRegister;
	using DeviceRegister::operator=;
	using DeviceRegister::operator API::data_t;

public:
	static constexpr API::data_t OBP1_DEFAULT_VALUE{0xFF};
};
} // Core

#endif // __LR35902_OBP1_H__