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
auto ADC_DATA = [](const data_t data)
{
	const uint16_t data_to_add = static_cast<uint16_t>(data) + static_cast<uint16_t>(F.IsSet(Flag::CARRY));
	const uint16_t reg_to_deduce = A_const;

	F.MutateByCondition(Tools::ZeroOnAddition(data_to_add, reg_to_deduce), Flag::ZERO);
	F.MutateByCondition(Tools::CarryOnAddition(reg_to_deduce, data_to_add), Flag::CARRY);
	F.Clear(Flag::SUB);
	F.MutateByCondition(Tools::HalfCarryOnAddition(reg_to_deduce, data_to_add), Flag::HALF_CARRY);

	A = (reg_to_deduce + data_to_add) & 0x00FF;
	return true;
};

// 0x88 ADC A,B
// Z 0 H C
auto ADC_0x88 = []()
{
	return ADC_DATA(B_const);
};

// 0x89 ADC A,C
// Z 0 H C
auto ADC_0x89 = []()
{
	return ADC_DATA(C_const);
};

// 0x8A ADC A,D
// Z 0 H C
auto ADC_0x8A = []()
{
	return ADC_DATA(D_const);
};

// 0x8B ADC A,E
// Z 0 H C
auto ADC_0x8B = []()
{
	return ADC_DATA(E_const);
};

// 0x8C ADC A,H
// Z 0 H C
auto ADC_0x8C = []()
{
	return ADC_DATA(H_const);
};

// 0x8D ADC A,L
// Z 0 H C
auto ADC_0x8D = []()
{
	return ADC_DATA(L_const);
};

// 0x8E ADC A,(HL)
// Z 0 H C
auto ADC_0x8E = []()
{
	return ADC_DATA(DataAt(HL_const));
};

// 0x8F ADC A,A
// Z 0 H C
auto ADC_0x8F = []()
{
	return ADC_DATA(A_const);
};

// 0xCE ADC A,d8
// Z 0 H C
auto ADC_0xCE = []()
{
	return ADC_DATA(D8());
};
} // Core

#endif // __LR35902_ADC_H__