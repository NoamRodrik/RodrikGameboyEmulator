/**
 * @file		RegisterEngine.h
 * @author		Noam Rodrik
 * @description LR35902 main register engine definition header.
 */
#ifndef __LR35902_REGISTER_ENGINE_H__
#define __LR35902_REGISTER_ENGINE_H__

#include <Core/Registers/Types/InstructionRegisters.h>
#include <Core/Registers/Types/StackRegisters.h>
#include <Core/Registers/Types/LogicRegisters.h>

namespace Core
{
class RegisterEngine
{
public:
	RegisterEngine(Bus& memory) : m_memory{memory} {}

	StackRegisterBase& GetStackRegister()
	{
		return this->m_stack_pointer;
	}

	AddressRegisterBase& GetProgramCounter()
	{
		return this->m_program_counter;
	}

	LogicRegisters& GetLogicRegisters()
	{
		return this->m_logic_registers;
	}

private:
	Bus&		   m_memory;
	LogicRegisters m_logic_registers{};
	SP_Register	   m_stack_pointer{0xFFFE, this->m_memory}; // 3.2.4. Stack Pointer
	PC_Register	   m_program_counter{0x0000};					// 3.2.3.Program Counter

private:
	/* Only processors can create register engines. */
	friend class Processor;

private:
	RegisterEngine(const RegisterEngine& other) = delete;
	RegisterEngine(const RegisterEngine&& other) = delete;
	RegisterEngine& operator=(const RegisterEngine& other) = delete;
	RegisterEngine& operator=(const RegisterEngine&& other) = delete;
};
} // Core

#endif // __LR35902_REGISTER_ENGINE_H_
