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

};

// 0x89 ADC A,C
// Z 0 H C
auto ADC_0x89 = []()
{

};

// 0x8A ADC A,D
// Z 0 H C
auto ADC_0x8A = []()
{

};

// 0x8B ADC A,E
// Z 0 H C
auto ADC_0x8B = []()
{

};

// 0x8C ADC A,H
// Z 0 H C
auto ADC_0x8C = []()
{

};

// 0x8D ADC A,L
// Z 0 H C
auto ADC_0x8D = []()
{

};

// 0x8E ADC A,(HL)
// Z 0 H C
auto ADC_0x8E = []()
{

};

// 0x8F ADC A,A
// Z 0 H C
auto ADC_0x8F = []()
{

};

// 0xCE ADC A,d8
// Z 0 H C
auto ADC_0xCE = []()
{

};
} // Core

#endif // __LR35902_ADC_H__