/**
 * @file		VBlank.h
 * @author		Noam Rodrik
 * @description LR35902 main vblank interrupt definition header.
 */
#ifndef __VBLANK_INTERRUPT_H__
#define __VBLANK_INTERRUPT_H__

#include <Tools/Tools.h>

namespace Core
{
inline const auto VBLANK_INTERRUPT = []()
{
	Message("Unimplemented!");
	return false;
};
} // Core

#endif // __VBLANK_INTERRUPT_H__