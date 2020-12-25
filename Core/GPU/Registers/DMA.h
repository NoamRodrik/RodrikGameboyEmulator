/**
 * @file		DMA.h
 * @author		Noam Rodrik
 * @description LR35902's main gpu dma definition header.
 */
#ifndef __LR35902_DMA_H__
#define __LR35902_DMA_H__

#include <Core/CPU/Registers/DeviceRegister.h>

namespace Core
{
/* Write only! */
class [[nodiscard]] DMA : public DeviceRegister<0xFF46>
{
public:
	using DeviceRegister::DeviceRegister;
	using DeviceRegister::operator=;
	using DeviceRegister::operator API::data_t;

public:
	static constexpr API::data_t DMA_DEFAULT_VALUE{0x00};
};
} // Core

#endif // __LR35902_DMA_H__