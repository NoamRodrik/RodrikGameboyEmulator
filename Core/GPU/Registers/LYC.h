/**
 * @file		LY.h
 * @author		Noam Rodrik
 * @description LR35902's main gpu lyc definition header.
 */
#ifndef __LR35902_LYC_H__
#define __LR35902_LYC_H__

#include <Core/CPU/Registers/DeviceRegister.h>

namespace Core
{
class [[nodiscard]] LYC : public DeviceRegister<0xFF45>
{
public:
	using DeviceRegister::DeviceRegister;
	using DeviceRegister::operator=;
	using DeviceRegister::operator API::data_t;

public:
	static constexpr API::data_t LYC_DEFAULT_VALUE{0x00};
};
} // Core

#endif // __LR35902_LYC_H__