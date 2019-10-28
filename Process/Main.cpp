/**
 * @file		Main.cpp
 * @author		Noam Rodrik
 * @description Program entry point
 */
#include <Core/Loader/GameLoader/GameLoader.h>
#include <Core/Processor/Processor.h>
using namespace Core;

int main()
{
	//GameLoader game_loader{"BootROM/cpu_instrs.gb"};
	GameLoader game_loader{ "BootROM/tetris.gb" };
	Processor::GetInstance().Run();

	return EXIT_SUCCESS;
}