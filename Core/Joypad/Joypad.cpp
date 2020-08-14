#include "Joypad.h"

#include <Core/CPU/Interrupts/InterruptHandler.h>
#include <Core/Joypad/Registers/JoypadRegister.h>

using namespace API;

namespace Core
{
void Joypad::PressButton(const EButton button)
{
	JoypadRegister joypad_register{static_cast<const data_t>(button)};

	// Button press occurred, call interrupt.
	InterruptHandler::IRQ(EInterrupts::H_L_P);
}
} // Core