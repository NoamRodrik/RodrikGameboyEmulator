/**
 * @file        SLA.h
 * @author      Noam Rodrik
 * @description Implementation to the SLA instruction(s).
 */
#ifndef __LR35902_SLA_H__
#define __LR35902_SLA_H__

#include <Core/CPU/Processor.h>
#include <Core/CPU/Instructions/Shortcuts.h>

using namespace API;

namespace Core
{
// SLA data
// Z 0 0 C
static constexpr auto SLA = [](data_t& data)
{
	F.MutateByCondition(Tools::IsBitSet(data, 7), Flag::CARRY);
	data <<= 1;
	F.MutateByCondition(data == 0, Flag::ZERO);
	F.Clear(Flag::SUB);
	F.Clear(Flag::HALF_CARRY);
	return true;
};

// 0x20 SLA B
// Z 0 0 C
static constexpr auto SLA_0x20 = []()
{
	return SLA(B);
};

// 0x21 SLA C
// Z 0 0 C
static constexpr auto SLA_0x21 = []()
{
	return SLA(C);
};

// 0x22 SLA D
// Z 0 0 C
static constexpr auto SLA_0x22 = []()
{
	return SLA(D);
};

// 0x23 SLA E
// Z 0 0 C
static constexpr auto SLA_0x23 = []()
{
	return SLA(E);
};

// 0x24 SLA H
// Z 0 0 C
static constexpr auto SLA_0x24 = []()
{
	return SLA(H);
};

// 0x25 SLA L
// Z 0 0 C
static constexpr auto SLA_0x25 = []()
{
	return SLA(L);
};

// 0x26 SLA (HL)
// Z 0 0 C
static constexpr auto SLA_0x26 = []()
{
	return RUN_COMMAND_ON_ADDRESS(HL_const, SLA);
};

// 0x27 SLA A
// Z 0 0 C
static constexpr auto SLA_0x27 = []()
{
	return SLA(A);
};
} // Core

#endif // __LR35902_SLA_H__