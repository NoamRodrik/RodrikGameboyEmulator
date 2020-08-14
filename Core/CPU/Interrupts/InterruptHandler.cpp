#include "InterruptHandler.h"
#include <Core/CPU/Interrupts/Registers/InterruptEnable.h>
#include <Core/CPU/Interrupts/Registers/InterruptFlag.h>
#include <Core/CPU/Instructions/General/Implementation/DI.h>
#include <Core/CPU/Instructions/Shortcuts.h>
#include <Core/CPU/Interrupts/SpecialRegisters/IME.h>
#include <Tools/Tools.h>

#include "Types/HighLowPinNumber.h"
#include "Types/Serial.h"
#include "Types/LCDC.h"
#include "Types/VBlank.h"
#include "Types/Timer.h"
#include <array>

using namespace API;

namespace Core
{
static const std::array<const Interrupt, 0x05> InterruptTable
{
	// Bit 0 : V - Blank
	Interrupt{VBLANK_INTERRUPT, 0x40, EInterrupts::VBLANK},

	// Bit 1 : LCDC (see STAT)
	Interrupt{LCDC_INTERRUPT, 0x48, EInterrupts::LCDC},

	// Bit 2 : Timer Overflow
	Interrupt{TIMER_INTERRUPT, 0x50, EInterrupts::TIMER},

	// Bit 3: Serial I / O transfer end
	Interrupt{SERIAL_INTERRUPT, 0x58, EInterrupts::SERIAL},
	
	// Bit 4: Transition from High to Low of Pin number P10-P13
	Interrupt{HIGH_LOW_PIN_NUMBER_INTERRUPT, 0x60, EInterrupts::H_L_P}
};
	
size_t InterruptHandler::ProcessInterrupts()
{
	// Interrupt Handling
	IME::EnableInterruptsIfScheduled();

	if (!IME::IsEnabled())
	{
		// The master enable isn't on, we can't use interrupts yet.
		return 0;
	}

	const auto* const interrupt_to_run = InterruptHandler::GetPrioritizedInterrupt();

	// If there's an interrupt.
	if (interrupt_to_run != nullptr)
	{
		// When any enabled interrupt is raised it will bring the CPU out of halt mode to service it, if required.
		Processor::ClearHalt();
		 
		// Clear interrupt.
		InterruptHandler::ClearInterrupt(interrupt_to_run->enum_value);

		// Disable all interrupts.
		DI_0xF3();
		
		// 1) Save PC
		SP.Push(PC);

		// 2) Acknowledge Interrupt
		SANITY(interrupt_to_run->Execute(), "Failed executing interrupts.");

		// 3) PC = Interrupt Service Routine
		PC = interrupt_to_run->jump_address;

		// It takes 5 MACHINE cycles to process this.
		return 20;
	}

	return 0;
}

void InterruptHandler::ClearInterrupt(const EInterrupts interrupt)
{
	InterruptFlag interrupt_requests{};
	interrupt_requests = interrupt_requests & (static_cast<data_t>(interrupt) ^ 0b11111111);
}

void InterruptHandler::IRQ(const EInterrupts interrupt)
{
	InterruptFlag interrupt_requests{};
	interrupt_requests = interrupt_requests | static_cast<data_t>(interrupt);
}

const Interrupt* const InterruptHandler::GetPrioritizedInterrupt()
{
	const InterruptEnable interrupt_enable{};
	const InterruptFlag interrupt_requests{};

	data_t interrupts_pending = interrupt_enable & interrupt_requests;

	// If there are no interrupts, return nullptr.
	if (interrupts_pending == 0)
	{
		return nullptr;
	}
	
	uint8_t chosen_interrupt = 0;
	
	while ((interrupts_pending & 0x01) == 0)
	{
		chosen_interrupt += 1;
		interrupts_pending >>= 1;
	}

	SANITY(chosen_interrupt < InterruptTable.size(), "Got an invalid choice!");
	return &InterruptTable[chosen_interrupt];
}
} // Core