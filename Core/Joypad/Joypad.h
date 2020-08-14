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
	enum EButton : API::data_t
	{
		RIGHT,
		LEFT,
		UP,
		DOWN,
		A,
		B,
		SELECT,
		START
	};

public:
	static void PressButton(const EButton button);
};
}

#endif // __JOYPAD_H__