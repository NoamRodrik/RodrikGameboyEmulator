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

class [[nodiscard]] Processor : public API::StaticInstance<Processor>
{
public:
	/* Returns the amount of clocks the operation needs. */
	[[nodiscard]] static const size_t Clock();
	[[nodiscard]] RegisterEngine& GetRegisters() { return this->_registers; }
	[[nodiscard]] Bus& GetMemory() { return this->_bus; }
	[[nodiscard]] gsl::not_null<IPPU*> GetPPU();

	[[nodiscard]] const RegisterEngine& GetRegisters() const { return this->_registers; }
	[[nodiscard]] const Bus& GetMemory() const { return this->_bus; }

	static inline void Stop() { Processor::GetInstance()._stop_request = true; }
	static inline void Halt() { Processor::GetInstance()._halted = true; }
	static inline void SetPrefixCommand() { Processor::GetInstance()._last_command_prefix = true; }

	[[nodiscard]] static inline bool IsStopped() { return Processor::GetInstance()._stop_request; }
	[[nodiscard]] static inline bool IsHalted() { return Processor::GetInstance()._halted; }
	[[nodiscard]] static inline bool IsPrefix() { return Processor::GetInstance()._last_command_prefix; }

	static inline void ClearPrefixCommand() { Processor::GetInstance()._last_command_prefix = false; }
	static inline void ClearStop() { Processor::GetInstance()._stop_request = false; }
	static inline void ClearHalt() { Processor::GetInstance()._halted = false; }

public:
	void LoadGame();

private:
	Processor() = default;
	~Processor() = default;

private:
	std::unique_ptr<IPPU> _ppu{nullptr};
	Bus					  _bus{};
	RegisterEngine		  _registers{_bus};
	/* Wait until a key is pressed on the screen! */
	bool				  _stop_request{false};
	/* Wait until an interrupt is done executing! */
	bool				  _halted{false};
	/* If the last command was PREFIX */
	bool				  _last_command_prefix{false};
	/* The global amount of cycles. */
	size_t				  _global_cycles{0};

private:
	friend class API::StaticInstance<Processor>;
};
} // Core

#endif // __PROCESSOR_H__
