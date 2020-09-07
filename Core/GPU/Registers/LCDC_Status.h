/**
 * @file		LCDC_Status.h
 * @author		Noam Rodrik
 * @description LR35902's main gpu lcdc status definition header.
 */
#ifndef __LR35902_LCDC_STATUS_H__
#define __LR35902_LCDC_STATUS_H__

#include <Core/CPU/Registers/MemoryRegister.h>

namespace Core
{
class LCDC_Status : public MemoryRegister<API::data_t>
{
public:
	constexpr LCDC_Status() : MemoryRegister{LCDC_ADDRESS} {}
	LCDC_Status(const API::data_t value) : MemoryRegister{LCDC_ADDRESS} { *this = value; }

public:
	using MemoryRegister::operator=;
	using MemoryRegister::operator API::data_t;

public:
	static constexpr API::address_t LCDC_ADDRESS{0xFF41};
	static constexpr API::address_t LCDC_DEFAULT_VALUE{0x00};
};
}

#endif // __LR35902_LCDC_STATUS_H__