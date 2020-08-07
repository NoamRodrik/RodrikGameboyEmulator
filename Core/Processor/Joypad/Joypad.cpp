#include "Joypad.h"

#include <Core/Processor/Joypad/Registers/JoypadRegister.h>
#include <Core/Interrupts/InterruptHandler.h>

namespace Core
{
void Joypad::PressButton(const EButton button)
{
	JoypadRegister joypad_register{static_cast<const data_t>(button)};

	// Button press occurred, call interrupt.
	InterruptHandler::IRQ(EInterrupts::H_L_P);
}
} // Core