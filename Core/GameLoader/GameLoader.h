/**
 * @file		GameLoader.h
 * @author		Noam Rodrik
 * @description Serves as a service to load games from the hard-disk.
 */
#ifndef __GAME_LOADER_H__
#define __GAME_LOADER_H__

#include <Core/Memory/Device/CartridgeRAM.h>
#include <Core/Processor/Processor.h>
#include <Tools/not_null.h>
#include <string>

namespace Core
{
class GameLoader
{
public:
	GameLoader(const std::string& game_path) : m_game_path{game_path}, m_game_file{fopen(game_path.c_str(), FILE_READ_MODE)}
	{
		auto* cartridge_ram_memory_ptr = Processor::GetInstance().GetMemory().GetDeviceAtAddress(CartridgeRAM::START_ADDRESS)->GetMemoryPointer();

		auto file_size = this->GetFileSize();

		// We might have a file size larger than the capable RAM size.
		if (file_size >= CartridgeRAM::SIZE)
		{
			file_size = CartridgeRAM::SIZE;
		}

		SANITY(fread(cartridge_ram_memory_ptr, 1, file_size, this->m_game_file) == file_size, "Failed reading from file!");
	}

	~GameLoader()
	{
		SANITY(fclose(this->m_game_file) == 0, "Failed closing game loader file ptr");
	}

private:
	const long GetFileSize() const
	{
		// Seek to end of file
		SANITY(fseek(this->m_game_file, 0, SEEK_END) == 0, "Failed seeking!");

		// get current file pointer
		auto size = ftell(m_game_file);

		// seek back to beginning of file
		SANITY(fseek(m_game_file, 0, SEEK_SET) == 0, "Failed seeking!");

		return size;
	}

private:
	static constexpr const char FILE_READ_MODE[] = "rb";

private:
	const std::string&   m_game_path;
	gsl::not_null<FILE*> m_game_file;
};
} // Core

#endif // __GAME_LOADER_H__