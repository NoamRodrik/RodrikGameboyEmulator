/**
 * @file		Joypad.h
 * @author		Noam Rodrik
 * @description A class that represents how the joypad looks and acts.
 */
#ifndef __JOYPAD_H__
#define __JOYPAD_H__

#include <Core/API/StaticInstance.h>
#include <Core/API/Definitions.h>

namespace Core
{
class Joypad : public StaticInstance<Joypad>
{
public:
	enum EButton : data_t
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