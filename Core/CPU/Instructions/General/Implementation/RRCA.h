/**
 * @file        RRCA.h
 * @author      Noam Rodrik
 * @description Implementation to the RRCA instruction(s).
 */
#ifndef __LR35902_RRCA_H__
#define __LR35902_RRCA_H__

#include <Core/CPU/Processor.h>
#include <Core/CPU/Instructions/Shortcuts.h>
#include <Core/CPU/Instructions/Prefix/Implementation/RRC.h>

using namespace API;

namespace Core
{
// 0x0F RRCA
// 0 0 0 C
static constexpr auto RRCA_0x0F = []()
{
	const bool RESULT = RRC_0x0F();
	F.Clear(Flag::ZERO);
	return RESULT;
};
} // Core

#endif // __LR35902_RRCA_H__