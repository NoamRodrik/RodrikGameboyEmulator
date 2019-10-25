/**
 * @file		Processor.h
 * @author		Noam Rodrik
 * @description LR35902's main processor definition header.
 */
#ifndef __PROCESSOR_H__
#define __PROCESSOR_H__

#include <Core/Registers/Logic/RegisterEngine.h>
#include <Core/API/Registers/RegisterPair.h>
#include <Tools/not_null.h>
#include <Core/Bus/Bus.h>

namespace Core
{
class Processor
{
public:
	// The processor is a singleton instance.
	static Processor& GetInstance();

	RegisterEngine& GetRegisters() { return this->m_registers; }
	Bus& GetMemory() { return this->m_bus; }

	const RegisterEngine& GetRegisters() const { return this->m_registers; }
	const Bus& GetMemory() const { return this->m_bus; }

	void Stop() { this->m_stop_request = true; }
	void Halt() { this->m_halted = true; }

	inline bool IsStopped() { return this->m_stop_request; }
	inline bool IsHalted() { return this->m_halted; }

private:
	Processor() = default;
	~Processor() = default;

private:
	inline static Processor* m_instance{nullptr};
	Bus						 m_bus;
	RegisterEngine			 m_registers{m_bus};
	/* Wait until a key is pressed on the screen! */
	bool					 m_stop_request{false};
	/* Wait until an interrupt is done executing! */
	bool					 m_halted{false};
};
} // Core

#endif // __PROCESSOR_H__
