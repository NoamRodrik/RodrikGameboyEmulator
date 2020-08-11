/**
 * @file		Shortcuts.h
 * @author		Noam Rodrik
 * @description A shortcut header file for all main registers
 *              and devices for the op-code implementation.
 */
#ifndef __SHORTCUTS_H__
#define __SHORTCUTS_H__

#include <Core/Processor/Processor.h>
#include <Core/API/Registers/RegisterPair.h>

namespace Core
{
// Global shortcuts
static Bus& memory{Processor::GetInstance().GetMemory()};
static const Bus& memory_const{Processor::GetInstance().GetMemory()};

static auto& A{Processor::GetInstance().GetRegisters().GetLogicRegisters().a};
static const auto& A_const{Processor::GetInstance().GetRegisters().GetLogicRegisters().a};
static auto& B{Processor::GetInstance().GetRegisters().GetLogicRegisters().b};
static const auto& B_const{Processor::GetInstance().GetRegisters().GetLogicRegisters().b};
static auto& C{Processor::GetInstance().GetRegisters().GetLogicRegisters().c};
static const auto& C_const{Processor::GetInstance().GetRegisters().GetLogicRegisters().c};
static auto& D{Processor::GetInstance().GetRegisters().GetLogicRegisters().d};
static const auto& D_const{Processor::GetInstance().GetRegisters().GetLogicRegisters().d};
static auto& E{Processor::GetInstance().GetRegisters().GetLogicRegisters().e};
static const auto& E_const{Processor::GetInstance().GetRegisters().GetLogicRegisters().e};
static auto& F{Processor::GetInstance().GetRegisters().GetLogicRegisters().f};
static const auto& F_const{Processor::GetInstance().GetRegisters().GetLogicRegisters().f};
static auto& H{Processor::GetInstance().GetRegisters().GetLogicRegisters().h};
static const auto& H_const{Processor::GetInstance().GetRegisters().GetLogicRegisters().h};
static auto& L{Processor::GetInstance().GetRegisters().GetLogicRegisters().l};
static const auto& L_const{Processor::GetInstance().GetRegisters().GetLogicRegisters().l};

static RegisterPair AF{A, F};
static const RegisterPair AF_const{A, F};
static RegisterPair BC{B, C};
static const RegisterPair BC_const{B, C};
static RegisterPair DE{D, E};
static const RegisterPair DE_const{D, E};
static RegisterPair HL{H, L};
static const RegisterPair HL_const{H, L};

static auto& SP{Processor::GetInstance().GetRegisters().GetStackRegister()};
static const auto& SP_const{Processor::GetInstance().GetRegisters().GetStackRegister()};
static auto& PC{Processor::GetInstance().GetRegisters().GetProgramCounter()};
static const auto& PC_const{Processor::GetInstance().GetRegisters().GetProgramCounter()};

const auto READ_DATA_AT = [](const auto& address)
{
	data_t data{0};
	SANITY(memory_const.Read(static_cast<const address_t>(address), data), "Failed reading from memory!");
	return data;
};

const auto RUN_COMMAND_ON_ADDRESS = [](const auto& address, const auto& command)
{
	data_t data{READ_DATA_AT(address)};
	const bool result = command(data);
	memory.Write(address, data);
	return result;
};

const auto PC_PARAMETER = [](const address_t offset = 1)
{
	return static_cast<const data_t>(READ_DATA_AT(PC_const + offset));
};

const auto D8 = []()
{
	return PC_PARAMETER();
};

const auto A8 = []()
{
	return D8();
};

const auto D16 = []()
{
	return (PC_PARAMETER(2) << 8) | PC_PARAMETER(1);
};

const auto A16 = []()
{
	return D16();
};

const auto R8 = []()
{
	return static_cast<const r8_t>(D8());
};
} // Core

#endif // __SHORTCUTS_H__