/**
 * @file		MainPixelEngine.h
 * @author		Noam Rodrik
 * @description The main class using the pixel game engine v2.0.8.
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
		RET_FALSE_IF_FAIL(this->Construct(SCREEN_WIDTH_PIXELS, SCREEN_HEIGHT_PIXELS, 4, 4), "Failed constructing pixel engine");
		this->_gpu_thread.reset(gsl::not_null<std::thread*>{new std::thread{&MainPixelEngine::Start, this}});

		return true;
	}

	virtual void Clock(std::size_t clock) override
	{
		SANITY(this->_render.Execute(clock), "Failed executing");
	}

private:
	virtual bool OnUserCreate() override
	{
		// Called once at startup, drawing white pixels.
		this->SetPixelMode(olc::Pixel::Mode::NORMAL);

		return this->FillPixels({WHITE_PIXEL[0], WHITE_PIXEL[1], WHITE_PIXEL[2]});
	}

	virtual bool OnUserUpdate(float) override
	{
		const time_point_t CURRENT{std::chrono::high_resolution_clock::now()};

		const time_duration_t SLEEP_LEFT_BETWEEN_FRAMES{
			std::max<float>(DELAYED_TIME - std::chrono::duration_cast<time_duration_t>(CURRENT - this->_previous).count(), 0)};

		std::this_thread::sleep_for(SLEEP_LEFT_BETWEEN_FRAMES);

		this->_previous = CURRENT;

		auto lcdc_control_register{LCDC_Control{}};
		auto lcdc_control{static_cast<LCDC_Control::Control>(lcdc_control_register)};
		RET_FALSE_IF_FAIL(lcdc_control.Validate(), "Invalid lcdc control");
		
		if (!lcdc_control.IsLCDEnabled())
		{
			// Disable LCD.
			static IORAM* io_ram_memory_ptr{static_cast<IORAM*>(this->_processor.GetMemory().GetDeviceAtAddress(LCDC_Status::LCDC_ADDRESS))};
			io_ram_memory_ptr->MaskLCDCStatus(0xFC);

			// Draw all black.
			RET_FALSE_IF_FAIL(this->FillPixels({BLACK_PIXEL[0], BLACK_PIXEL[1], BLACK_PIXEL[2]}), "Failed drawing black onto screen");
			RET_FALSE_IF_FAIL(this->_render.ResetLCD(), "Failed resetting LCD");
			return true;
		}

		return this->Render();
	}

	bool Render()
	{
		auto& screen = this->_render.GetScreen();

		for (int32_t height = 0; height < SCREEN_HEIGHT_PIXELS; ++height)
		{
			for (int32_t width = 0; width < SCREEN_WIDTH_PIXELS; ++width)
			{
				RET_FALSE_IF_FAIL(this->Draw(width, height, screen[height][width]), "Failed drawing pixel");
			}
		}

		return true;
	}

	virtual Processor& GetProcessor() override
	{
		return this->_processor;
	}

private:
	bool FillPixels(olc::Pixel color)
	{
		for (int32_t x = 0; x < ScreenWidth(); ++x)
		{
			for (int32_t y = 0; y < ScreenHeight(); ++y)
			{
				RET_FALSE_IF_FAIL(Draw(x, y, color), "Failed to draw pixel");
			}
		}

		return true;
	}

private:
	using time_point_t = std::chrono::time_point<std::chrono::high_resolution_clock>;
	using time_duration_t = std::chrono::duration<float, std::milli>;

public:
	static constexpr float FRAMES_PER_SECOND{57.3f};
	static constexpr float DELAYED_TIME{1000.0f / FRAMES_PER_SECOND};

private:
	time_point_t			     _previous{std::chrono::high_resolution_clock::now()};
	Processor&					 _processor;
	std::unique_ptr<std::thread> _gpu_thread{nullptr};
	LCDRender					 _render{this->_processor.GetMemory(), *this};
};
}

#endif // __GPU_ENGINE_MAIN_PIXEL_ENGINE_H__