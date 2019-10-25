from lxml.html import parse

page = parse("http://pastraiser.com/cpu/gameboy/gameboy_opcodes.html")
rows = page.xpath("body/table")[0].findall("tr")

with open("InstructionLookupTable.h", 'w+') as our_file:
  our_file.write("""/**
 * @file		InstructionLookupTable.h
 * @author		Noam Rodrik
 * @description A lookup table for all instruction sequences.
 */
#ifndef __INSTRUCTION_LOOKUP_TABLE_H__
#define __INSTRUCTION_LOOKUP_TABLE_H__

#include <Core/Processor/Instruction/Instruction.h>
#include <Tools/Tools.h>
#include <array>

/* Instruction includes */
""")

  short_name_table = []
  for row in range(1, 17):
    for column in range(1, 17):
      try:
        c = rows[row][column].text_content()
        s = c.split('\xa0')
        operation_name = s[0][:-1]
        short_name = operation_name.split(' ')[0]

        if not short_name in short_name_table:
          short_name_table.append(short_name)
      except:
        pass
  
  for short_name in short_name_table:
    if not len(short_name) == 0:
      our_file.write('#include <Core/Processor/Instruction/Generated/{}.h>\n'.format(short_name))
  
  our_file.write('\n')
  our_file.write('namespace Core\n')
  our_file.write('{\n')
  our_file.write("""auto TERMINATE = []()
{
	STOP_RUNNING("An error has occurred!");
};
""")
  our_file.write("""
Message("Complete the prefix table!");
Message("Need to increment PC after each instruction!");
static const std::array<Instruction, 0x10 * 0x10> INSTRUCTION_LOOKUP_TABLE =
""")
  our_file.write('{\n')

  for row in range(1, 17):
    for column in range(1, 17):
      ident = "0x" + str(format(row - 1, 'X')) + str(format(column - 1, 'X'))
      try:
        #	// 0x00 NOP
	      # Instruction{NOP_0x00, 1, 4},
        c = rows[row][column].text_content()
        s = c.split('\xa0')
        operation_name = s[0][:-1]
        flags = c[-7:]
        bytes_size = s[0][-1:]
        cycles_amount = s[2][:-7]

        if (len(cycles_amount.split('/')) > 1):
          str_cycles_amount = ', '.join(cycles_amount.split('/'))
        else:
          str_cycles_amount = ", ".join(cycles_amount.split('/')) + ", 0"

        short_name = operation_name.split(' ')[0]
        our_file.write("    // " + ident + " " + operation_name + "\n")
        our_file.write("    // " + flags + "\n")
        our_file.write("    Instruction{" + short_name + "_" + ident + ", " + str(bytes_size) + ", " + str(str_cycles_amount) + ", " + "\"" + str(operation_name) + "\"" + "}")

      except:
        our_file.write("    // " + ident + " INVALID\n")
        our_file.write("    Instruction{TERMINATE, 0, 0, 0, \"ERROR\"}")

      if (ident != "0xFF"):
        our_file.write(',\n')
      our_file.write('\n')

  our_file.write('};\n')
  our_file.write('} // Core\n')
  our_file.write('\n')
  our_file.write('#endif // __INSTRUCTION_LOOKUP_TABLE_H__')
