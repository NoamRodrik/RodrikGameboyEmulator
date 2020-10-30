/**
 * @file		InterruptEnable.h
 * @author		Noam Rodrik
 * @description LR35902 main interrupt enable definition header.
 */
#ifndef __LR35902_INTERRUPT_ENABLE_H__
#define __LR35902_INTERRUPT_ENABLE_H__

#include <Core/CPU/Registers/DeviceRegister.h>

namespace Core
{
class [[nodiscard]] InterruptEnable : public DeviceRegister<0xFFFF>
{
public:
	using DeviceRegister::DeviceRegister;
	using DeviceRegister::operator=;
	using DeviceRegister::operator API::data_t;

public:
	static constexpr API::data_t INTERRUPT_ENABLE_DEFAULT_VALUE{0x00};
};
} // Core

#endif // __LR35902_INTERRUPT_ENABLE_H__