/**
 * @file		Processor.h
 * @author		Noam Rodrik
 * @description LR35902's main processor definition header.
 */
#ifndef __PROCESSOR_H__
#define __PROCESSOR_H__

#include <Core/Registers/Logic/RegisterEngine.h>
#include <Core/API/StaticInstance.h>
#include <Core/Bus/Bus.h>

namespace Core
{
class Instruction;

class Processor : public StaticInstance<Processor>
{
public:
	/* Returns the amount of clocks the operation needs. */
	static const size_t Clock();

	RegisterEngine& GetRegisters() { return this->m_registers; }
	Bus& GetMemory() { return this->m_bus; }

	const RegisterEngine& GetRegisters() const { return this->m_registers; }
	const Bus& GetMemory() const { return this->m_bus; }

	void Stop() { this->m_stop_request = true; }
	void Halt() { this->m_halted = true; }
	void SetPrefixCommand() { this->m_last_command_prefix = true; }

	inline bool IsStopped() { return this->m_stop_request; }
	inline bool IsHalted() { return this->m_halted; }
	inline bool IsPrefix() { return this->m_last_command_prefix; }

	void ClearPrefixCommand() { this->m_last_command_prefix = false; }
	void ClearStop() { this->m_stop_request = false; }
	void ClearHalt() { this->m_halted = false; }

private:
	friend class StaticInstance<Processor>;
	Processor() = default;
	~Processor() = default;

#if _DEBUG
private:
	static void PrintInstruction(const Instruction& instruction_to_print);
	static void PrintRegisters();
	static void PrintFlags();
#endif

private:
	inline static Processor* m_instance{nullptr};
	Bus						 m_bus;
	RegisterEngine			 m_registers{m_bus};
	/* Wait until a key is pressed on the screen! */
	bool					 m_stop_request{false};
	/* Wait until an interrupt is done executing! */
	bool					 m_halted{false};
	/* If the last command was PREFIX */
	bool					 m_last_command_prefix{false};
};
} // Core

#endif // __PROCESSOR_H__
