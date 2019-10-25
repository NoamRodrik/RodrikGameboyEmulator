/**
 * @file        RST.h
 * @author      Noam Rodrik
 * @description Implementation to the RST instruction(s).
 */
#ifndef __LR35902_RST_H__
#define __LR35902_RST_H__

#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>

namespace Core
{
// 0xC7 RST 00H
// - - - -
auto RST_0xC7 = []()
{

};

// 0xCF RST 08H
// - - - -
auto RST_0xCF = []()
{

};

// 0xD7 RST 10H
// - - - -
auto RST_0xD7 = []()
{

};

// 0xDF RST 18H
// - - - -
auto RST_0xDF = []()
{

};

// 0xE7 RST 20H
// - - - -
auto RST_0xE7 = []()
{

};

// 0xEF RST 28H
// - - - -
auto RST_0xEF = []()
{

};

// 0xF7 RST 30H
// - - - -
auto RST_0xF7 = []()
{

};

// 0xFF RST 38H
// - - - -
auto RST_0xFF = []()
{

};
} // Core

#endif // __LR35902_RST_H__