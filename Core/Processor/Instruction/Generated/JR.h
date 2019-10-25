/**
 * @file        JR.h
 * @author      Noam Rodrik
 * @description Implementation to the JR instruction(s).
 */
#ifndef __LR35902_JR_H__
#define __LR35902_JR_H__

#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>

namespace Core
{
// 0x18 JR r8
// - - - -
auto JR_0x18 = []()
{

};

// 0x20 JR NZ,r8
// - - - -
auto JR_0x20 = []()
{

};

// 0x28 JR Z,r8
// - - - -
auto JR_0x28 = []()
{

};

// 0x30 JR NC,r8
// - - - -
auto JR_0x30 = []()
{

};

// 0x38 JR C,r8
// - - - -
auto JR_0x38 = []()
{

};
} // Core

#endif // __LR35902_JR_H__