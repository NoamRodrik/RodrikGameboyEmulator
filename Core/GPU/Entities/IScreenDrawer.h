/**
 * @file		IScreenDrawer.h
 * @author		Noam Rodrik
 * @description LR35902 interface for screen drawer
 */
#ifndef __LR35902_ISCREEN_DRAWER_H__
#define __LR35902_ISCREEN_DRAWER_H__

#include <Core/GPU/Entities/PixelRow.h>
#include <API/Definitions.h>
#include <cstddef>

namespace Core
{
class IScreenDrawer
{
public:
	virtual ~IScreenDrawer() = default;

public:
	virtual bool DrawPalette(int32_t x, int32_t y, PaletteColor color) = 0;
	virtual bool DrawPixel(int32_t x, int32_t y, PixelColor color) = 0;

public:
	virtual void SetYCoordinate(API::data_t y) = 0;
};
}

#endif // __LR35902_ISCREEN_DRAWER_H__