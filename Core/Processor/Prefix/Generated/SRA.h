/**
 * @file        SRA.h
 * @author      Noam Rodrik
 * @description Implementation to the SRA instruction(s).
 */
#ifndef __LR35902_SRA_H__
#define __LR35902_SRA_H__

#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>

namespace Core
{
// 0x28 SRA B
// Z 0 0 0
auto SRA_0x28 = []()
{
  return true;
};

// 0x29 SRA C
// Z 0 0 0
auto SRA_0x29 = []()
{
  return true;
};

// 0x2A SRA D
// Z 0 0 0
auto SRA_0x2A = []()
{
  return true;
};

// 0x2B SRA E
// Z 0 0 0
auto SRA_0x2B = []()
{
  return true;
};

// 0x2C SRA H
// Z 0 0 0
auto SRA_0x2C = []()
{
  return true;
};

// 0x2D SRA L
// Z 0 0 0
auto SRA_0x2D = []()
{
  return true;
};

// 0x2E SRA (HL)
// Z 0 0 0
auto SRA_0x2E = []()
{
  return true;
};

// 0x2F SRA A
// Z 0 0 0
auto SRA_0x2F = []()
{
  return true;
};
} // Core

#endif // __LR35902_SRA_H__