/**
 * @file		LogicRegisters.h
 * @author		Noam Rodrik
 * @description LR35902 main header for manipulating logical registers.
 */
#ifndef __LR35902_LOGIC_REGISTERS_H__
#define __LR35902_LOGIC_REGISTERS_H__

#include <Core/Registers/Types/ArithmeticRegisters.h>
#include <Core/Registers/Types/EFLRegisters.h>

namespace Core
{
#pragma pack(push, 1)
struct LogicRegisters
{
	ArithmeticRegister a{0};
	EFL_Register	   f{0};
	ArithmeticRegister b{0};
	ArithmeticRegister c{0};
	ArithmeticRegister d{0};
	ArithmeticRegister e{0};
	ArithmeticRegister h{0};
	ArithmeticRegister l{0};
};
#pragma pack(pop)

static_assert(sizeof(LogicRegisters) == sizeof(data_t) * 8);
} // Core

#endif // __LR35902_LOGIC_REGISTERS_H__
