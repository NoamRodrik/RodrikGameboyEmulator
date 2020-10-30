/**
 * @file		IPPU.h
 * @author		Noam Rodrik
 * @description The interface of the PPU.
 */
#ifndef __CORE_IPPU_H__
#define __CORE_IPPU_H__

#include <Core/GPU/Definitions.h>
#include <cstddef>

namespace Core
{
class [[nodiscard]] IPPU
{
public:
	virtual ~IPPU() = default;

public:
	[[nodiscard]] virtual bool Startup() = 0;
	virtual void Clock(std::size_t clock) = 0;
	[[nodiscard]] virtual Processor& GetProcessor() = 0;
	[[nodiscard]] virtual bool IsLCDEnabled() const = 0;
	virtual void DisableLCD() = 0;
	virtual void EnableLCD() = 0;
	[[nodiscard]] virtual PPUState GetState() const = 0;
	virtual void InitiateDMA() = 0;
	[[nodiscard]] virtual bool CheckOnceDMAOccurred() = 0;
};
} // Core

#endif // __CORE_IPPU_H__