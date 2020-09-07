/**
 * @file		BGP.h
 * @author		Noam Rodrik
 * @description LR35902's main gpu bgp definition header.
 */
#ifndef __LR35902_BGP_H__
#define __LR35902_BGP_H__

#include <Core/CPU/Registers/MemoryRegister.h>

namespace Core
{
class BGP : public MemoryRegister<API::data_t>
{
public:
	constexpr BGP() : MemoryRegister{BGP_ADDRESS} {}
	BGP(const API::data_t value) : MemoryRegister{BGP_ADDRESS} { *this = value; }

public:
	using MemoryRegister::operator=;
	using MemoryRegister::operator API::data_t;

public:
	static constexpr API::address_t BGP_ADDRESS{0xFF47};
	static constexpr API::address_t BGP_DEFAULT_VALUE{0xFC};
};
}

#endif // __LR35902_BGP_H__