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
#include <Core/Bus/Devices/IORAM.h>
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
	MainPixelEngine(Processor& processor) : _processor{processor}
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
		RET_FALSE_IF_FAIL(this->Construct(SCREEN_WIDTH_PIXELS, SCREEN_HEIGHT_PIXELS, 3, 3), "Failed constructing pixel engine");
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
	virtual void Wait() const override
	{
		Message("Wait cancelled");
		//while (PixelGameEngine::bAtomTrigger);
	}

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
		Message("TODO!");
		/*auto lcdc_register{LCDC_Control{}};
		auto lcdc_control{static_cast<LCDC_Control::Control>(lcdc_register)};

		RET_FALSE_IF_FAIL(lcdc_control.Validate(), "Failed validating lcdc control");

		if (lcdc_control.IsLCDEnabled())
		{*/
		return this->_render.Execute(std::exchange(this->_clock, 0));
		/*}

		return true;*/
	}

	virtual bool DrawPalette(int32_t x, int32_t y, PaletteColor color) override
	{
		return DrawPixel(x, y, PaletteMap::ColorOf(color));
	}

	virtual bool DrawPixel(int32_t x, int32_t y, PixelColor color) override
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
				MAIN_LOG("Got an invalid pixel color: %u", static_cast<uint32_t>(color));
				return false;
			}
		}
	}

	/**
	 * Setting the value through LY will reset the counter.
	 */
	virtual void SetYCoordinate(API::data_t y) override
	{
		static_cast<IORAM*>(this->_processor.GetMemory().GetDeviceAtAddress(LY::LY_ADDRESS))->GetMemoryPointer()[LY::LY_ADDRESS - IORAM::START_ADDRESS] = y;
	}

private:
	Processor&						   _processor;
	std::array<Tile, API::CANVAS_SIZE> _canvas{};
	std::unique_ptr<std::thread>       _gpu_thread{nullptr};
	LCDRender                          _render{this->_processor.GetMemory(), *this};
	std::size_t                        _clock{0};
};
}

#endif // __GPU_ENGINE_MAIN_PIXEL_ENGINE_H__