/**
 * @file		PixelFIFO.h
 * @author		Noam Rodrik
 * @description LR35902 pixel FIFO header
 */
#ifndef __LR35902_PIXEL_FIFO_H__
#define __LR35902_PIXEL_FIFO_H__

#include <Core/GPU/Entities/PixelRowContainer.h>
#include <Core/GPU/Mechanics/Fetcher.h>
#include <API/Definitions.h>
#include <Tools/Tools.h>

namespace Core
{
/**
 * The pixel FIFO works as a pixel priority mechanic.
 * It's in charge of all points in the PPU (from background, to sprites...).
 */
class PixelFIFO
{
public:
	constexpr PixelFIFO() = default;
	~PixelFIFO() = default;

private:
	PixelRowContainer _first_row{};
	PixelRowContainer _second_row{};
	Fetcher           _fetcher{};
};
}

#endif // __LR35902_PIXEL_FIFO_H__