﻿/**
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
#include <Core/Joypad/Joypad.h>
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
		if (this->IsLCDEnabled())
		{
			SANITY(this->_render.Execute(clock), "Failed executing");
		}
	}

	virtual bool IsLCDEnabled() const override
	{
		return this->_enabled;
	}

	virtual void DisableLCD() override
	{
		this->_enabled = false;
		this->_render.ResetLCD();
	}

	virtual void EnableLCD() override
	{
		this->_enabled = true;
		this->_render.Start();
	}

	virtual PPUState GetState() const override
	{
		return this->_render._state;
	}

private:
	virtual bool OnUserCreate() override
	{
		// Called once at startup, drawing white pixels.
		this->SetPixelMode(olc::Pixel::Mode::NORMAL);
		this->EnableLCD();
		return true;
	}

	virtual bool OnUserUpdate(float) override
	{
		const time_point_t CURRENT{std::chrono::high_resolution_clock::now()};

		const time_duration_t SLEEP_LEFT_BETWEEN_FRAMES{
			std::max<float>(DELAYED_TIME - std::chrono::duration_cast<time_duration_t>(CURRENT - this->_previous).count(), 0)};

		std::this_thread::sleep_for(SLEEP_LEFT_BETWEEN_FRAMES);

		this->_previous = CURRENT;

		this->HandleButtonPress();
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

	void HandleButtonPress() const
	{
		API::data_t direction_status{0x00};
		API::data_t button_status{0x00};

		if (this->GetKey(olc::Key::ESCAPE).bPressed ||
			this->GetKey(olc::Key::ESCAPE).bHeld ||
			this->GetKey(olc::Key::ESCAPE).bReleased)
		{
			Tools::SetBit(button_status, static_cast<std::size_t>(Joypad::Control::START));
		}

		if (this->GetKey(olc::Key::ENTER).bPressed ||
			this->GetKey(olc::Key::ENTER).bHeld ||
			this->GetKey(olc::Key::ENTER).bReleased)
		{
			Tools::SetBit(button_status, static_cast<std::size_t>(Joypad::Control::SELECT));
		}

		if (this->GetKey(olc::Key::W).bPressed ||
			this->GetKey(olc::Key::UP).bPressed ||
			this->GetKey(olc::Key::W).bHeld ||
			this->GetKey(olc::Key::UP).bHeld ||
			this->GetKey(olc::Key::W).bReleased ||
			this->GetKey(olc::Key::UP).bReleased)
		{
			Tools::SetBit(direction_status, static_cast<std::size_t>(Joypad::Control::UP));
		}

		if (this->GetKey(olc::Key::S).bPressed ||
			this->GetKey(olc::Key::DOWN).bPressed ||
			this->GetKey(olc::Key::S).bHeld ||
			this->GetKey(olc::Key::DOWN).bHeld ||
			this->GetKey(olc::Key::S).bReleased ||
			this->GetKey(olc::Key::DOWN).bReleased)
		{
			Tools::SetBit(direction_status, static_cast<std::size_t>(Joypad::Control::DOWN));
		}

		if (this->GetKey(olc::Key::A).bPressed ||
			this->GetKey(olc::Key::LEFT).bPressed ||
			this->GetKey(olc::Key::A).bHeld ||
			this->GetKey(olc::Key::LEFT).bHeld ||
			this->GetKey(olc::Key::A).bReleased ||
			this->GetKey(olc::Key::LEFT).bReleased)
		{
			Tools::SetBit(direction_status, static_cast<std::size_t>(Joypad::Control::LEFT));
		}

		if (this->GetKey(olc::Key::D).bPressed ||
			this->GetKey(olc::Key::RIGHT).bPressed ||
			this->GetKey(olc::Key::D).bHeld ||
			this->GetKey(olc::Key::RIGHT).bHeld ||
			this->GetKey(olc::Key::D).bReleased ||
			this->GetKey(olc::Key::RIGHT).bReleased)
		{
			Tools::SetBit(direction_status, static_cast<std::size_t>(Joypad::Control::RIGHT));
		}

		if (this->GetKey(olc::Key::J).bPressed ||
			this->GetKey(olc::Key::J).bHeld ||
			this->GetKey(olc::Key::J).bReleased)
		{
			Tools::SetBit(button_status, static_cast<std::size_t>(Joypad::Control::B));
		}

		if (this->GetKey(olc::Key::K).bPressed ||
			this->GetKey(olc::Key::K).bHeld ||
			this->GetKey(olc::Key::K).bReleased)
		{
			Tools::SetBit(button_status, static_cast<std::size_t>(Joypad::Control::A));
		}

		Joypad::ChangeStatus(direction_status, button_status);
	}

private:
	using time_point_t = std::chrono::time_point<std::chrono::high_resolution_clock>;
	using time_duration_t = std::chrono::duration<float, std::milli>;

public:
	static constexpr float FRAMES_PER_SECOND{59.7f};
	static constexpr float DELAYED_TIME{1000.0f / FRAMES_PER_SECOND};

private:
	time_point_t			     _previous{std::chrono::high_resolution_clock::now()};
	Processor&					 _processor;
	std::unique_ptr<std::thread> _gpu_thread{nullptr};
	LCDRender					 _render{this->_processor.GetMemory(), *this};
	std::atomic<bool>            _enabled{true};
};
}

#endif // __GPU_ENGINE_MAIN_PIXEL_ENGINE_H__