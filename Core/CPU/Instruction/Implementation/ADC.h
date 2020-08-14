/**
 * @file        ADC.h
 * @author      Noam Rodrik
 * @description Implementation to the ADC instruction(s).
 */
#ifndef __LR35902_ADC_H__
#define __LR35902_ADC_H__

#include <Core/CPU/Processor.h>
#include <Core/CPU/Instruction/Shortcuts.h>

using namespace API;

namespace Core
{
// ADC A, data
// Z 0 H C
auto ADC = [](const data_t data)
{
	const bool WAS_CARRY_SET{F.IsSet(Flag::CARRY)};

	F.Clear(Flag::SUB);
	F.MutateByCondition((static_cast<int32_t>(A_const & 0x0F) + static_cast<int32_t>(data & 0x0F) + static_cast<int32_t>(WAS_CARRY_SET)) > 0x0F, Flag::HALF_CARRY);
	F.MutateByCondition((static_cast<int32_t>(A_const & 0xFF) + static_cast<int32_t>(data & 0xFF) + static_cast<int32_t>(WAS_CARRY_SET)) > 0xFF, Flag::CARRY);

	A = A_const + data + static_cast<const data_t>(WAS_CARRY_SET);

	F.MutateByCondition(A_const == 0, Flag::ZERO);

	return true;
};

// 0x88 ADC A,B
// Z 0 H C
static constexpr auto ADC_0x88 = []()
{
	return ADC(B_const);
};

// 0x89 ADC A,C
// Z 0 H C
static constexpr auto ADC_0x89 = []()
{
	return ADC(C_const);
};

// 0x8A ADC A,D
// Z 0 H C
static constexpr auto ADC_0x8A = []()
{
	return ADC(D_const);
};

// 0x8B ADC A,E
// Z 0 H C
static constexpr auto ADC_0x8B = []()
{
	return ADC(E_const);
};

// 0x8C ADC A,H
// Z 0 H C
static constexpr auto ADC_0x8C = []()
{
	return ADC(H_const);
};

// 0x8D ADC A,L
// Z 0 H C
static constexpr auto ADC_0x8D = []()
{
	return ADC(L_const);
};

// 0x8E ADC A,(HL)
// Z 0 H C
static constexpr auto ADC_0x8E = []()
{
	return ADC(READ_DATA_AT(HL_const));
};

// 0x8F ADC A,A
// Z 0 H C
static constexpr auto ADC_0x8F = []()
{
	return ADC(A_const);
};

// 0xCE ADC A,d8
// Z 0 H C
static constexpr auto ADC_0xCE = []()
{
	return ADC(D8());
};
} // Core

#endif // __LR35902_ADC_H__