/**
 * @file		InterruptHandler.h
 * @author		Noam Rodrik
 * @description LR35902 main interrupt handler definition header.
 */
#ifndef __LR35902_INTERRUPT_HANDLER_H__
#define __LR35902_INTERRUPT_HANDLER_H__

#include <Core/Interrupts/Interrupt.h>
#include <Core/API/Definitions.h>

namespace Core
{
class InterruptHandler
{
public:
	static size_t ProcessInterrupts();

	static void IRQ(const EInterrupts interrupt);
	static void ClearInterrupt(const EInterrupts interrupt);

private:
	static const Interrupt* const GetPrioritizedInterrupt();
};
} // Core

#endif // __LR35902_INTERRUPT_HANDLER_H__
