/**
 * @file		Joypad.cpp
 * @author		Noam Rodrik
 * @description Joypad implementation for LR35902
 */
#include <Core/Joypad/Joypad.h>
#include <Core/CPU/Interrupts/InterruptHandler.h>
#include <Core/Joypad/Registers/JoypadRegister.h>
#include <Core/Bus/Devices/IORAM.h>

using namespace API;

namespace Core
{
void Joypad::ChangeStatus(const API::data_t direction_status, const API::data_t button_status)
{
	SANITY(static_cast<API::data_t>(direction_status) <= 0x0F &&
		   static_cast<API::data_t>(button_status) <= 0x0F, "Invalid joypad parameters");

	if (direction_status == 0 && Joypad::GetDirectionStatus() != 0 ||
		direction_status != 0 && Joypad::GetDirectionStatus() == 0 ||
		button_status == 0 && Joypad::GetButtonStatus() != 0 ||
		button_status != 0 && Joypad::GetButtonStatus() == 0)
	{
		// Button change occurred, run irq.
		InterruptHandler::IRQ(EInterrupts::H_L_P);
	}

	// Invert the controls.
	// Since, in H_L_O, if a button is pressed - it turns into 0!
	Joypad::GetInstance()._direction_status = (~direction_status) & 0x0F;
	Joypad::GetInstance()._button_status = (~button_status) & 0x0F;
}

const API::data_t Joypad::GetButtonStatus()
{
	return Joypad::GetInstance()._button_status;
}

const API::data_t Joypad::GetDirectionStatus()
{
	return Joypad::GetInstance()._direction_status;
}
} // Core