/**
 * @file        ADC.h
 * @author      Noam Rodrik
 * @description Implementation to the ADC instruction(s).
 */
#ifndef __LR35902_ADC_H__
#define __LR35902_ADC_H__

#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>

namespace Core
{
// ADC A, data
// Z 0 H C
auto ADC_WITH_A = [](const data_t data)
{
	const bool WAS_CARRY_SET{F.IsSet(Flag::CARRY)};
	
	const int32_t ADC_RESULT{static_cast<int32_t>(A_const) + static_cast<int32_t>(data) + WAS_CARRY_SET};

	F.MutateByCondition(static_cast<const data_t>(ADC_RESULT) == 0, Flag::ZERO);
	F.Clear(Flag::SUB);
	F.MutateByCondition(((A_const & 0x0F) + (data & 0x0F) + (static_cast<const data_t>(WAS_CARRY_SET) & 0x0F)) > 0x0F, Flag::HALF_CARRY);
	F.MutateByCondition(ADC_RESULT > 0xFF, Flag::CARRY);

	A = static_cast<const data_t>(ADC_RESULT);
	return true;
};

// 0x88 ADC A,B
// Z 0 H C
auto ADC_0x88 = []()
{
	return ADC_WITH_A(B_const);
};

// 0x89 ADC A,C
// Z 0 H C
auto ADC_0x89 = []()
{
	return ADC_WITH_A(C_const);
};

// 0x8A ADC A,D
// Z 0 H C
auto ADC_0x8A = []()
{
	return ADC_WITH_A(D_const);
};

// 0x8B ADC A,E
// Z 0 H C
auto ADC_0x8B = []()
{
	return ADC_WITH_A(E_const);
};

// 0x8C ADC A,H
// Z 0 H C
auto ADC_0x8C = []()
{
	return ADC_WITH_A(H_const);
};

// 0x8D ADC A,L
// Z 0 H C
auto ADC_0x8D = []()
{
	return ADC_WITH_A(L_const);
};

// 0x8E ADC A,(HL)
// Z 0 H C
auto ADC_0x8E = []()
{
	return ADC_WITH_A(READ_DATA_AT(HL_const));
};

// 0x8F ADC A,A
// Z 0 H C
auto ADC_0x8F = []()
{
	return ADC_WITH_A(A_const);
};

// 0xCE ADC A,d8
// Z 0 H C
auto ADC_0xCE = []()
{
	return ADC_WITH_A(D8());
};
} // Core

#endif // __LR35902_ADC_H__