/**
 * @file		LCDC.h
 * @author		Noam Rodrik
 * @description LR35902 main LCDC interrupt definition header.
 */
#ifndef __LCDC_INTERRUPT_H__
#define __LCDC_INTERRUPT_H__

#include <Tools/Tools.h>

namespace Core
{
inline const auto LCDC_INTERRUPT = []()
{
	return false;
};
} // Core

#endif // __LCDC_INTERRUPT_H__