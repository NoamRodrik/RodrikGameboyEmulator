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
// 0x20 SLA B
// Z 0 0 C
auto SLA_0x20 = []()
{
  return true;
};

// 0x21 SLA C
// Z 0 0 C
auto SLA_0x21 = []()
{
  return true;
};

// 0x22 SLA D
// Z 0 0 C
auto SLA_0x22 = []()
{
  return true;
};

// 0x23 SLA E
// Z 0 0 C
auto SLA_0x23 = []()
{
  return true;
};

// 0x24 SLA H
// Z 0 0 C
auto SLA_0x24 = []()
{
  return true;
};

// 0x25 SLA L
// Z 0 0 C
auto SLA_0x25 = []()
{
  return true;
};

// 0x26 SLA (HL)
// Z 0 0 C
auto SLA_0x26 = []()
{
  return true;
};

// 0x27 SLA A
// Z 0 0 C
auto SLA_0x27 = []()
{
  return true;
};
} // Core

#endif // __LR35902_SLA_H__