/**
 * @file        JP.h
 * @author      Noam Rodrik
 * @description Implementation to the JP instruction(s).
 */
#ifndef __LR35902_JP_H__
#define __LR35902_JP_H__

#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>

namespace Core
{
// 0xC2 JP NZ,a16
// - - - -
auto JP_0xC2 = []()
{

};

// 0xC3 JP a16
// - - - -
auto JP_0xC3 = []()
{

};

// 0xCA JP Z,a16
// - - - -
auto JP_0xCA = []()
{

};

// 0xD2 JP NC,a16
// - - - -
auto JP_0xD2 = []()
{

};

// 0xDA JP C,a16
// - - - -
auto JP_0xDA = []()
{

};

// 0xE9 JP (HL)
// - - - -
auto JP_0xE9 = []()
{

};
} // Core

#endif // __LR35902_JP_H__