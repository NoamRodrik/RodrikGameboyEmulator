/**
 * @file		BGP.h
 * @author		Noam Rodrik
 * @description LR35902's main gpu bgp definition header.
 */
#ifndef __LR35902_BGP_H__
#define __LR35902_BGP_H__

#include <Core/CPU/Registers/DeviceRegister.h>

namespace Core
{
class [[nodiscard]] BGP : public DeviceRegister<0xFF47>
{
public:
	using DeviceRegister::DeviceRegister;
	using DeviceRegister::operator=;
	using DeviceRegister::operator API::data_t;

public:
	static constexpr API::data_t BGP_DEFAULT_VALUE{0xFC};
};
} // Core

#endif // __LR35902_BGP_H__