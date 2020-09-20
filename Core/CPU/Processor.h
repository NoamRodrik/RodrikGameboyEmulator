/**
 * @file		Processor.h
 * @author		Noam Rodrik
 * @description LR35902's main processor definition header.
 */
#ifndef __PROCESSOR_H__
#define __PROCESSOR_H__

#include <Core/CPU/Registers/RegisterEngine.h>
#include <API/StaticInstance.h>
#include <Core/GPU/IPPU.h>
#include <Core/Bus/Bus.h>

namespace Core
{
class Instruction;

class Processor : public API::StaticInstance<Processor>
{
public:
	/* Returns the amount of clocks the operation needs. */
	static const size_t Clock();
	RegisterEngine& GetRegisters() { return this->m_registers; }
	Bus& GetMemory() { return this->m_bus; }
	gsl::not_null<IPPU*> GetPPU();

	const RegisterEngine& GetRegisters() const { return this->m_registers; }
	const Bus& GetMemory() const { return this->m_bus; }

	static inline void Stop() { Processor::GetInstance().m_stop_request = true; }
	static inline void Halt() { Processor::GetInstance().m_halted = true; }
	static inline void SetPrefixCommand() { Processor::GetInstance().m_last_command_prefix = true; }

	static inline bool IsStopped() { return Processor::GetInstance().m_stop_request; }
	static inline bool IsHalted() { return Processor::GetInstance().m_halted; }
	static inline bool IsPrefix() { return Processor::GetInstance().m_last_command_prefix; }

	static inline void ClearPrefixCommand() { Processor::GetInstance().m_last_command_prefix = false; }
	static inline void ClearStop() { Processor::GetInstance().m_stop_request = false; }
	static inline void ClearHalt() { Processor::GetInstance().m_halted = false; }

public:
	void LoadGame();

private:
	Processor() = default;
	~Processor() = default;

#if _DEBUG
private:
	static void PrintInstruction(const Instruction& instruction_to_print);
	static void PrintRegisters();
	static void PrintFlags();
	static void PrintInterruptRegisters();
#endif

private:
	std::unique_ptr<IPPU> m_ppu{nullptr};
	Bus					  m_bus{};
	RegisterEngine		  m_registers{m_bus};
	/* Wait until a key is pressed on the screen! */
	bool				  m_stop_request{false};
	/* Wait until an interrupt is done executing! */
	bool				  m_halted{false};
	/* If the last command was PREFIX */
	bool				  m_last_command_prefix{false};
	/* The global amount of cycles. */
	size_t				  m_global_cycles{0};

private:
	friend class API::StaticInstance<Processor>;
};
} // Core

#endif // __PROCESSOR_H__
