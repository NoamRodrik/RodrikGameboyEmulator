/**
 * @file		MainPixelEngine.cpp
 * @author		Noam Rodrik
 * @description The main class using the pixel game engine v2.0.7.
 */
#ifndef __GPU_ENGINE_MAIN_PIXEL_ENGINE_H__
#define __GPU_ENGINE_MAIN_PIXEL_ENGINE_H__

#define OLC_PGE_APPLICATION

#pragma warning( push, 0 )
#include <Contrib/PixelGameEngine/OLCPixelGameEngine.h>
#pragma warning ( pop )

namespace Core
{
class MainPixelEngine : public olc::PixelGameEngine
{
public:
	MainPixelEngine()
	{
		// Name you application
		sAppName = "RodrikGameBoyEmulator";
	}

	virtual ~MainPixelEngine() = default;

public:
	virtual bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		return true;
	}

	virtual bool OnUserUpdate(float fElapsedTime) override
	{
		for (int x = 0; x < ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++)
				Draw(x, y, olc::Pixel(x % 256, y % 256, (x*y) % 256));
		return true;
	}
};
}

#endif // __GPU_ENGINE_MAIN_PIXEL_ENGINE_H__