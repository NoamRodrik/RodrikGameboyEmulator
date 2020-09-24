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
#include <API/Memory/Device/IMemoryDeviceAccess.h>
#include <Core/GPU/Registers/LCDC_Control.h>
#include <Core/GPU/Mechanics/LCDRender.h>
#include <Core/GPU/Entities/PaletteMap.h>
#include <Core/GPU/Entities/Tile.h>
#include <Core/GPU/Definitions.h>
#include <API/Definitions.h>
#include <Core/GPU/IPPU.h>
#include <Tools/Tools.h>
#include <array>
#pragma warning ( pop )

namespace Core
{
class MainPixelEngine : public olc::PixelGameEngine, public IPPU
{
public:
	MainPixelEngine(API::IMemoryDeviceAccess& memory) : _memory{memory}
	{
		// Name your application
		sAppName = ENGINE_WINDOW_NAME;
	}

	virtual ~MainPixelEngine() = default;

public:
	virtual bool Startup() override
	{
#ifndef NO_PIXEL_ENGINE
#ifdef FULL_PALETTE_TEST
		RET_FALSE_IF_FAIL(this->Construct(256, 256, 3, 3), "Failed constructing pixel engine");
#else
		RET_FALSE_IF_FAIL(this->Construct(160, 144, 3, 3), "Failed constructing pixel engine");
#endif
		this->_gpu_thread.reset(gsl::not_null<std::thread*>{new std::thread{&MainPixelEngine::StartWithoutUpdate, this}});
#endif

		return true;
	}

	virtual void Clock(std::size_t clock) override
	{
		this->_clock += clock;
		this->TriggerUpdate();
	}

private:
	virtual bool OnUserCreate() override
	{
		// Called once at startup, drawing white pixels.
		for (int32_t x = 0; x < ScreenWidth(); ++x)
		{
			for (int32_t y = 0; y < ScreenHeight(); ++y)
			{
				RET_FALSE_IF_FAIL(DrawPixel(x, y, PixelColor::WHITE), "Failed to draw pixel");
			}
		}

		return true;
	}

	virtual bool OnUserUpdate(float) override
	{
		Message("TODO: The background needs to be divided between clocks.");
		Message("TODO: The canvas isn't supposed to be entirely drawn on each epoch.");
		/*
		if (!this->DrawBackground())
		{
			MAIN_LOG("Failed drawing background");
		}

		if (!this->DrawCanvas())
		{
			MAIN_LOG("Failed drawing canvas");
		}
		*/

		return this->_render.Execute(std::exchange(this->_clock, 0));
	}

	bool DrawBackground()
	{
		auto lcdc_register{LCDC_Control{}};
		auto lcdc_control{static_cast<LCDC_Control::Control>(lcdc_register)};

		RET_FALSE_IF_FAIL(lcdc_control.Validate(), "Failed validating lcdc control");

		// If the background has been enabled.
		if (lcdc_control.IsBackgroundEnabled())
		{
			static_assert(sizeof(this->_canvas) / sizeof(this->_canvas[0]) >= BACKGROUND_MAP_SIZE, "Canvas is too small for background mapping size");

			// Here we start drawing the background.
			for (API::address_t canvas_slot = 0; canvas_slot < BACKGROUND_MAP_SIZE; ++canvas_slot)
			{
				API::data_t data{0x00};
				RET_FALSE_IF_FAIL(this->_memory.Read(canvas_slot + lcdc_control.GetBackgroundMapStart(), data), "Failed reading memory for canvas");
				RET_FALSE_IF_FAIL(canvas_slot < this->_canvas.size(), "Invalid indexing");

				API::address_t address_to_load_tile{lcdc_control.GetTileSelectOffset()};
				address_to_load_tile += lcdc_control.IsSigned() && data > 127 ? -1 * static_cast<int32_t>(data) : static_cast<int32_t>(data);
				RET_FALSE_IF_FAIL(this->_canvas[canvas_slot].LoadTile(address_to_load_tile), "Failed to load tile!");
			}
		}

		return true;
	}

	bool DrawCanvas()
	{
		for (int32_t current_height = 0; current_height < API::CANVAS_HEIGHT; ++current_height)
		{
			for (API::data_t current_pixel_row = 0; current_pixel_row < Tile::HEIGHT_PIXELS; ++current_pixel_row)
			{
				for (int32_t current_width = 0; current_width < API::CANVAS_WIDTH; ++current_width)
				{
					RET_FALSE_IF_FAIL(current_height * API::CANVAS_HEIGHT + current_width < this->_canvas.size(), "Buffer overflow");
					RET_FALSE_IF_FAIL(this->DrawPixelRow(current_width * Tile::HEIGHT_PIXELS, current_height * Tile::HEIGHT_PIXELS + current_pixel_row,
										this->_canvas[current_height * API::CANVAS_HEIGHT + current_width].GetPixelRow(current_pixel_row)),
						              "Failed to draw pixel row!");
				}
			}
		}

		return true;
	}

	bool DrawPixelRow(int32_t x, int32_t y, PixelRow pixel_row)
	{
		for (int32_t pixel_index = 7; pixel_index >= 0; --pixel_index)
		{
			RET_FALSE_IF_FAIL(this->DrawPalette(x + pixel_index, y, pixel_row.GetColorByIndex(pixel_index)),
				             "Failed drawing pixel row (%d, %d)", x + pixel_index, y);
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

private:
	API::IMemoryDeviceAccess&		   _memory;
	std::array<Tile, API::CANVAS_SIZE> _canvas{};
	std::unique_ptr<std::thread>       _gpu_thread{nullptr};
	LCDRender                          _render{};
	std::size_t                        _clock{0};
};
}

#endif // __GPU_ENGINE_MAIN_PIXEL_ENGINE_H__