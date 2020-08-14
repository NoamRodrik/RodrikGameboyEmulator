/**
 * @file		GameLoader.h
 * @author		Noam Rodrik
 * @description Serves as a service to load games from the hard-disk.
 */
#ifndef __GAME_LOADER_H__
#define __GAME_LOADER_H__

#include <Core/Cartridge/Loader/BinaryLoader.h>
#include <Core/Bus/Devices/CartridgeRAM.h>
#include <Core/CPU/Processor.h>
#include <API/Definitions.h>
#include <Contrib/GSL/not_null.h>
#include <string>

namespace Core
{
class GameLoader : public BinaryLoader
{
public:
	GameLoader(const std::string& game_path) : BinaryLoader{game_path,
															static_cast<CartridgeRAM*>(Processor::GetInstance().GetMemory().GetDeviceAtAddress(CartridgeRAM::START_ADDRESS))->GetMemoryPointer(),
															static_cast<long>(CartridgeRAM::SIZE)}
	{}

	virtual ~GameLoader() = default;
};
} // Core

#endif // __GAME_LOADER_H__