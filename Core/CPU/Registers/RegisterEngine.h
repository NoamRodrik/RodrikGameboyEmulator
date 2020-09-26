/**
 * @file		RegisterEngine.h
 * @author		Noam Rodrik
 * @description LR35902 main register engine definition header.
 */
#ifndef __LR35902_REGISTER_ENGINE_H__
#define __LR35902_REGISTER_ENGINE_H__

#include <Core/CPU/Registers/Types/InstructionRegisters.h>
#include <Core/CPU/Registers/Types/StackRegisters.h>
#include <Core/CPU/Registers/Types/LogicRegisters.h>

namespace Core
{
class RegisterEngine
{
public:
	RegisterEngine(Bus& memory) : _memory{memory} {}

	API::StackRegisterBase& GetStackRegister()
	{
		return this->_stack_pointer;
	}

	API::AddressRegisterBase& GetProgramCounter()
	{
		return this->_program_counter;
	}

	LogicRegisters& GetLogicRegisters()
	{
		return this->_logic_registers;
	}

private:
	Bus&		   _memory;
	LogicRegisters _logic_registers{};
	SP_Register	   _stack_pointer{0xFFFE, this->_memory}; // 3.2.4. Stack Pointer
	PC_Register	   _program_counter{0x0000}; // 3.2.3.Program Counter

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
