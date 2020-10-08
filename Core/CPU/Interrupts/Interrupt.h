/**
 * @file		Interrupt.h
 * @author		Noam Rodrik
 * @description LR35902 main interrupt definition header.
 */
#ifndef __LR35902_INTERRUPT_H__
#define __LR35902_INTERRUPT_H__

#include <API/Definitions.h>
#include <functional>
#include <cstdint>

namespace Core
{
enum class EInterrupts : API::data_t
{
	VBLANK = 1 << 0,
	LCDC   = 1 << 1,
	TIMER  = 1 << 2,
	SERIAL = 1 << 3,
	H_L_P  = 1 << 4
};
	
class [[nodiscard]] Interrupt
{
public:
	inline bool Execute() const { return this->func(); }

public:
	const std::function<bool()> func{nullptr};
	const uint32_t jump_address{0};
	const EInterrupts enum_value;
};
} // Core

#endif