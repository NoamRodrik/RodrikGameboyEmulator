/**
 * @file        CALL.h
 * @author      Noam Rodrik
 * @description Implementation to the CALL instruction(s).
 */
#ifndef __LR35902_CALL_H__
#define __LR35902_CALL_H__

#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>

namespace Core
{
// 0xC4 CALL NZ,a16
// - - - -
auto CALL_0xC4 = []()
{

};

// 0xCC CALL Z,a16
// - - - -
auto CALL_0xCC = []()
{

};

// 0xCD CALL a16
// - - - -
auto CALL_0xCD = []()
{

};

// 0xD4 CALL NC,a16
// - - - -
auto CALL_0xD4 = []()
{

};

// 0xDC CALL C,a16
// - - - -
auto CALL_0xDC = []()
{

};
} // Core

#endif // __LR35902_CALL_H__