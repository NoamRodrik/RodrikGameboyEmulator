/**
 * @file		OBP0.h
 * @author		Noam Rodrik
 * @description LR35902's main gpu obp0 definition header.
 */
#ifndef __LR35902_OBP0_H__
#define __LR35902_OBP0_H__

#include <Core/CPU/Registers/DeviceRegister.h>

namespace Core
{
class [[nodiscard]] OBP0 : public DeviceRegister<0xFF48>
{
public:
	using DeviceRegister::DeviceRegister;
	using DeviceRegister::operator=;
	using DeviceRegister::operator API::data_t;

public:
	static constexpr API::data_t OBP0_DEFAULT_VALUE{0xFF};
};
} // Core

#endif // __LR35902_OBP0_H__