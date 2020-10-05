/**
 * @file		Joypad.h
 * @author		Noam Rodrik
 * @description A class that represents how the joypad looks and acts.
 */
#ifndef __JOYPAD_H__
#define __JOYPAD_H__

#include <API/StaticInstance.h>
#include <API/Definitions.h>

namespace Core
{
class Joypad : public API::StaticInstance<Joypad>
{
public:
	enum class Control : API::data_t
	{
		RIGHT = 0, 
		LEFT = 1,
		UP = 2,
		DOWN = 3,
		A = 0, 
		B = 1,
		SELECT = 2,
		START = 3
	};

	Message("Check order");
	enum class Mode : API::data_t
	{
		SELECT_BUTTONS = 4,
		SELECT_DIRECTIONS = 5
	};

public:
	static void ChangeStatus(const API::data_t direction_status, const API::data_t button_status);

public:
	static const API::data_t GetButtonStatus();
	static const API::data_t GetDirectionStatus();

private:
	API::data_t _direction_status{0x00};
	API::data_t _button_status{0x00};
};
} // Core

#endif // __JOYPAD_H__