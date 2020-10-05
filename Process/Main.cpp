/**
 * @file		Main.cpp
 * @author		Noam Rodrik
 * @description Program entry point
 */
#include <Core/CPU/Interrupts/SpecialRegisters/IME.h>
#include <Core/CPU/Processor.h>
#include <Core/Clock/Clock.h>

using namespace Core;

int main(int argc, char** argv)
{
	Processor::GetInstance().LoadGame();
	Processor::GetInstance().GetPPU()->Startup();

	while (true)
	{
		// CPU needs to syncronize clocks.
		for (std::size_t current_cycle = Processor::Clock(); current_cycle > 0; --current_cycle)
		{
			Clock::SyncClock();
		}
	}

	return EXIT_SUCCESS;
}