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
// 0x88 ADC A,B
// Z 0 H C
auto ADC_0x88 = []()
{
	return true;
};

// 0x89 ADC A,C
// Z 0 H C
auto ADC_0x89 = []()
{
	return true;
};

// 0x8A ADC A,D
// Z 0 H C
auto ADC_0x8A = []()
{
	return true;
};

// 0x8B ADC A,E
// Z 0 H C
auto ADC_0x8B = []()
{
	return true;
};

// 0x8C ADC A,H
// Z 0 H C
auto ADC_0x8C = []()
{
	return true;
};

// 0x8D ADC A,L
// Z 0 H C
auto ADC_0x8D = []()
{
	return true;
};

// 0x8E ADC A,(HL)
// Z 0 H C
auto ADC_0x8E = []()
{
	return true;
};

// 0x8F ADC A,A
// Z 0 H C
auto ADC_0x8F = []()
{
	return true;
};

// 0xCE ADC A,d8
// Z 0 H C
auto ADC_0xCE = []()
{
	return true;
};
} // Core

#endif // __LR35902_ADC_H__