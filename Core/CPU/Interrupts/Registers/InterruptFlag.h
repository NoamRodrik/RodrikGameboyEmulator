/**
 * @file		InterruptFlag.h
 * @author		Noam Rodrik
 * @description LR35902 main interrupt flag definition header.
 */
#ifndef __LR35902_INTERRUPT_FLAG_H__
#define __LR35902_INTERRUPT_FLAG_H__

#include <Core/CPU/Registers/DeviceRegister.h>

namespace Core
{
class [[nodiscard]] InterruptFlag : public DeviceRegister<0xFF0F>
{
public:
	using DeviceRegister::DeviceRegister;
	using DeviceRegister::operator=;
	using DeviceRegister::operator API::data_t;

public:
	static constexpr API::data_t INTERRUPT_FLAG_DEFAULT_VALUE{0x00};
};
} // Core

#endif // __LR35902_INTERRUPT_FLAG_H__