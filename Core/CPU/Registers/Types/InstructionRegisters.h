/**
 * @file		InstructionRegisters.h
 * @author		Noam Rodrik
 * @description LR35902 main instruction registers definition header.
 */
#ifndef __LR35902_INSTRUCTION_REGISTERS_H__
#define __LR35902_INSTRUCTION_REGISTERS_H__

#include <API/Registers/RegisterBase.h>

namespace Core
{
class PC_Register : public API::AddressRegisterBase
{
public:
	using API::AddressRegisterBase::AddressRegisterBase;
	using API::AddressRegisterBase::RegisterType;

public:
	inline void Next() { this->m_data += 1; }
};

static_assert(sizeof(PC_Register) == sizeof(PC_Register::RegisterType));
} // Core

#endif // __LR35902_INSTRUCTION_REGISTERS_H__
