/**
 * @file		Main.cpp
 * @author		Noam Rodrik
 * @description Program entry point
 */
#include <Core/Cartridge/Loader/GameLoader/GameLoader.h>
#include <Core/CPU/Interrupts/SpecialRegisters/IME.h>
#include <Core/GPU/Engine/MainPixelEngine.h>
#include <Core/CPU/Timers/Timer.h>
#include <Core/CPU/Processor.h>
#include <Core/Clock/Clock.h>
#include <filesystem>

using namespace Core;

void LoadGame();

int main(int argc, char** argv)
{
#ifndef NO_PIXEL_ENGINE
	MainPixelEngine pixel_engine{};
	SANITY(pixel_engine.Construct(160, 144, 4, 4), "Failed constructing pixel engine");
	pixel_engine.Start();
#endif

	LoadGame();

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

void LoadGame()
{
	// Choose ROMS from GB folder
	size_t index{1};
	for (const auto& file : std::filesystem::directory_iterator("TestROM"))
	{
		MAIN_LOG("%llu) %s", index++, file.path().string().c_str());
	}

	uint32_t chosen_index{0};
	do
	{
		MAIN_LOG("Choose a wanted file from the file list.");
		scanf_s("%u", &chosen_index);
	} while (chosen_index >= index || chosen_index < 1);
	index = chosen_index;

	auto directory_iterator = std::filesystem::directory_iterator("TestROM");
	while (index > 1)
	{
		++directory_iterator;
		--index;
	}

	GameLoader game_loader{ (*directory_iterator).path().string() };
}