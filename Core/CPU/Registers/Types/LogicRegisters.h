/**
 * @file		LogicRegisters.h
 * @author		Noam Rodrik
 * @description LR35902 main header for manipulating logical registers.
 */
#ifndef __LR35902_LOGIC_REGISTERS_H__
#define __LR35902_LOGIC_REGISTERS_H__

#include <Core/CPU/Registers/Types/ArithmeticRegisters.h>
#include <Core/CPU/Registers/Types/EFLRegisters.h>

namespace Core
{
#pragma pack(push, 1)
struct LogicRegisters
{
	ArithmeticRegister a{0x01};
	EFL_Register	   f{0xB0};
	ArithmeticRegister b{0x00};
	ArithmeticRegister c{0x13};
	ArithmeticRegister d{0x00};
	ArithmeticRegister e{0xD8};
	ArithmeticRegister h{0x01};
	ArithmeticRegister l{0x4D};
};
#pragma pack(pop)

static_assert(sizeof(LogicRegisters) == sizeof(API::data_t) * 8);
} // Core

#endif // __LR35902_LOGIC_REGISTERS_H__
