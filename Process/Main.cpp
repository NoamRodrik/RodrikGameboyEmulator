/**
 * @file		Main.cpp
 * @author		Noam Rodrik
 * @description Program entry point
 */
#include <Core/Loader/GameLoader/GameLoader.h>
#include <Core/Processor/Timer/Timer.h>
#include <Core/Processor/Clock/Clock.h>
#include <Core/Processor/Processor.h>
#include <filesystem>
#include <SDL.h>

using namespace Core;

void LoadGame();

int main(int argc, char** argv)
{
#ifndef NO_SDL
	// Initialize Simple DirectMedia Library for video rendering, audio, and keyboard events
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Renderer* renderer = nullptr;
	SDL_Window* window = nullptr;
	SDL_CreateWindowAndRenderer(160 * 3, 144 * 3, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE, &window, &renderer);
	SDL_SetWindowTitle(window, "Rodrik Gameboy Emulator");
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

	// Clear screen with White
	SDL_RenderClear(renderer);

	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, 160, 144);
#endif

	LoadGame();

	size_t clock_cycle = 0;
	while (true)
	{
		Message("Implement!");
		if (clock_cycle > 0)
		{
			clock_cycle -= 1;
			continue;
		}

		// CPU needs to clock.
		Clock::SyncClock();
		clock_cycle = Processor::Clock();
	}
	
#ifndef NO_SDL
	// De-initialize.
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
#endif

	return EXIT_SUCCESS;
}

void LoadGame()
{
	// Choose ROMS from GB folder
	size_t index{0};
	for (const auto& file : std::filesystem::directory_iterator("BootROM"))
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

	auto directory_iterator = std::filesystem::directory_iterator("BootROM");
	while (index > 0)
	{
		++directory_iterator;
		--index;
	}

	GameLoader game_loader{ (*directory_iterator).path().string() };
}