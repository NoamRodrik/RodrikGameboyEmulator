/**
 * @file		Main.cpp
 * @author		Noam Rodrik
 * @description Program entry point
 */
#include <Core/CPU/Interrupts/SpecialRegisters/IME.h>
#include <Core/GPU/Engine/MainPixelEngine.h>
#include <Core/CPU/Timers/Timer.h>
#include <Core/CPU/Processor.h>
#include <Core/Clock/Clock.h>

using namespace Core;

int main(int argc, char** argv)
{
#ifndef NO_PIXEL_ENGINE
	MainPixelEngine pixel_engine{};
	SANITY(pixel_engine.Construct(160, 144, 4, 4), "Failed constructing pixel engine");
	pixel_engine.Start();
#endif

	Processor::GetInstance().LoadGame();

	while (true)
	{
		// CPU needs to syncronize clocks.
		for (size_t current_cycle = Processor::Clock(); current_cycle > 0; --current_cycle)
		{
			Clock::SyncClock();
		}
	}

	return EXIT_SUCCESS;
}