/**
 * @file		MainPixelEngine.h
 * @author		Noam Rodrik
 * @description The main class using the pixel game engine v2.0.7.
 */
#ifndef __GPU_ENGINE_MAIN_PIXEL_ENGINE_H__
#define __GPU_ENGINE_MAIN_PIXEL_ENGINE_H__

#define OLC_PGE_APPLICATION

#pragma warning( push, 0 )
#include <Contrib/PixelGameEngine/OLCPixelGameEngine.h>
#include <Core/GPU/Registers/LCDC_Control.h>
#include <Core/GPU/Entities/PaletteMap.h>
#include <Core/GPU/Definitions.h>
#include <API/Definitions.h>
#include <Core/GPU/IPPU.h>
#include <Tools/Tools.h>
#pragma warning ( pop )

namespace Core
{
class MainPixelEngine : public olc::PixelGameEngine, public IPPU
{
public:
	MainPixelEngine()
	{
		// Name your application
		sAppName = "RodrikGameBoyEmulator";
	}

	virtual ~MainPixelEngine() = default;

public:
	virtual bool Load() override
	{
		return this->LoadTileset();
	}

	virtual bool Startup() override
	{
#ifndef NO_PIXEL_ENGINE
		RET_FALSE_IF_FAIL(this->Construct(160, 144, 3, 3), "Failed constructing pixel engine");
		this->Start();
#endif

		return true;
	}

private:
	virtual bool OnUserCreate() override
	{
		// Called once at startup, drawing white pixels.
		for (int32_t x = 0; x < ScreenWidth(); ++x)
		{
			for (int32_t y = 0; y < ScreenHeight(); ++y)
			{
				SANITY(DrawPixel(x, y, PixelColor::WHITE), "Failed to draw pixel");
			}
		}

		return true;
	}

	virtual bool OnUserUpdate(float fElapsedTime) override
	{
		return DrawBackground();
	}

	bool LoadTileset()
	{
		LCDC_Control lcdc_control{};
		
		API::address_t start_address{0x00};
		API::address_t end_address{0x00};

		if (static_cast<LCDC_Control::Control>(lcdc_control).background_map_select == LCDC_Control::Control::BACKGROUND_MAP_9800_9BFF)
		{
			start_address = 0x9800;
			end_address = 0x9BFF;
		}
		else if (static_cast<LCDC_Control::Control>(lcdc_control).background_map_select == LCDC_Control::Control::BACKGROUND_MAP_9C00_9FFF)
		{
			start_address = 0x9C00;
			end_address = 0x9FFF;
		}
		else
		{
			LOG("Got an invalid valid for background map select of the LCDC control");
			return false;
		}

		Message("TODO: Load tileset");
		return true;
	}

	bool DrawBackground()
	{
		LCDC_Control lcdc_control{};

		// If the background has been enabled.
		if (static_cast<LCDC_Control::Control>(lcdc_control).background_enable == LCDC_Control::Control::BACKGROUND_ON)
		{
			// Here we start drawing the background.
		}

		return true;
	}

	bool DrawPalette(int32_t x, int32_t y, PaletteColor color)
	{
		return DrawPixel(x, y, PaletteMap::ColorOf(color));
	}

	bool DrawPixel(int32_t x, int32_t y, PixelColor color)
	{	
		switch (color)
		{
			case (PixelColor::WHITE):
			{
				return Draw(x, y, olc::Pixel{255, 255, 255});
			}

			case (PixelColor::BLACK):
			{
				return Draw(x, y, olc::Pixel{0, 0, 0});
			}

			case (PixelColor::LIGHT_GREY):
			{
				return Draw(x, y, olc::Pixel{221, 180, 180});
			}

			case (PixelColor::DARK_GREY):
			{
				return Draw(x, y, olc::Pixel{97, 79, 76});
			}

			default:
			{
				LOG("Got an invalid pixel color: %u", static_cast<uint32_t>(color));
				return false;
			}
		}
	}
};
}

#endif // __GPU_ENGINE_MAIN_PIXEL_ENGINE_H__