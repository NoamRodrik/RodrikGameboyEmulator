/**
 * @file        SLA.h
 * @author      Noam Rodrik
 * @description Implementation to the SLA instruction(s).
 */
#ifndef __LR35902_SLA_H__
#define __LR35902_SLA_H__

#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>

namespace Core
{
// SLA reg
// Z 0 0 0
auto SLA_REG = [](auto& reg)
{
	F.MutateByCondition(reg == 0, Flag::ZERO);
	F.Clear(Flag::SUB);
	F.Clear(Flag::HALF_CARRY);
	F.Clear(Flag::CARRY);
	reg <<= 1;
	reg |= static_cast<data_t>(1);
	return true;
};

// 0x20 SLA B
// Z 0 0 C
auto SLA_0x20 = []()
{
	return SLA_REG(B);
};

// 0x21 SLA C
// Z 0 0 C
auto SLA_0x21 = []()
{
	return SLA_REG(C);
};

// 0x22 SLA D
// Z 0 0 C
auto SLA_0x22 = []()
{
	return SLA_REG(D);
};

// 0x23 SLA E
// Z 0 0 C
auto SLA_0x23 = []()
{
	return SLA_REG(E);
};

// 0x24 SLA H
// Z 0 0 C
auto SLA_0x24 = []()
{
	return SLA_REG(H);
};

// 0x25 SLA L
// Z 0 0 C
auto SLA_0x25 = []()
{
	return SLA_REG(L);
};

// 0x26 SLA (HL)
// Z 0 0 C
auto SLA_0x26 = []()
{
	return RunCommandAtAddress(HL_const, SLA_REG);
};

// 0x27 SLA A
// Z 0 0 C
auto SLA_0x27 = []()
{
	return SLA_REG(A);
};
} // Core

#endif // __LR35902_SLA_H__