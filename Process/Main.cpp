/**
 * @file		Main.cpp
 * @author		Noam Rodrik
 * @description Program entry point
 */
#include <Core/Loader/GameLoader/GameLoader.h>
#include <Core/GPU/Engine/MainPixelEngine.h>
#include <Core/Processor/Timer/Timer.h>
#include <Core/Processor/Clock/Clock.h>
#include <Core/Processor/Processor.h>
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

	do
	{
		size_t clock_cycle{Processor::Clock()};

		// CPU needs to syncronize clocks.
		for (size_t current_cycle = 0; current_cycle <= clock_cycle; ++current_cycle)
		{
			Clock::SyncClock();
		}
	} while (true);

	return EXIT_SUCCESS;
}

void LoadGame()
{
	// Choose ROMS from GB folder
	size_t index{0};
	for (const auto& file : std::filesystem::directory_iterator("TestROM"))
	{
		MAIN_LOG("%llu) %s", index++, file.path().string().c_str());
	}

	int chosen_index{0};
	do
	{
		MAIN_LOG("Choose a wanted file from the file list.");
		chosen_index = getchar() - '0';
		static_cast<void>(getchar());
	} while (chosen_index >= index || chosen_index < 0);

	index = chosen_index;

	auto directory_iterator = std::filesystem::directory_iterator("TestROM");
	while (index > 0)
	{
		++directory_iterator;
		--index;
	}

	GameLoader game_loader{ (*directory_iterator).path().string() };
}