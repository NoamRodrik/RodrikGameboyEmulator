/**
 * @file		DMA.h
 * @author		Noam Rodrik
 * @description LR35902's main gpu dma definition header.
 */
#ifndef __LR35902_DMA_H__
#define __LR35902_DMA_H__

#include <Core/CPU/Registers/MemoryRegister.h>

namespace Core
{
/* Write only! */
class DMA : public MemoryRegister<API::data_t>
{
public:
	constexpr DMA() : MemoryRegister{DMA_ADDRESS} {}
	DMA(const API::data_t value) : MemoryRegister{DMA_ADDRESS} { *this = value; }

public:
	using MemoryRegister::operator=;
	using MemoryRegister::operator API::data_t;

public:
	static constexpr API::address_t DMA_ADDRESS{0xFF46};
	static constexpr API::address_t DMA_DEFAULT_VALUE{0x00};
};
} // Core

#endif // __LR35902_DMA_H__