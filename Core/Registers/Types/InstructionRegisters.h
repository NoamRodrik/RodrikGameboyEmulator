/**
 * @file		InstructionRegisters.h
 * @author		Noam Rodrik
 * @description LR35902 main instruction registers definition header.
 */
#ifndef __LR35902_INSTRUCTION_REGISTERS_H__
#define __LR35902_INSTRUCTION_REGISTERS_H__

#include <Core/Registers/RegisterBase.h>

namespace Core
{
class PC_Register : public AddressRegisterBase
{
public:
	using AddressRegisterBase::AddressRegisterBase;
	using AddressRegisterBase::RegisterType;

public:
	inline void Next() { this->m_data += 1; }
};

static_assert(sizeof(PC_Register) == sizeof(PC_Register::RegisterType));
} // Core

#endif // __LR35902_INSTRUCTION_REGISTERS_H__
