from lxml.html import parse

page = parse("http://pastraiser.com/cpu/gameboy/gameboy_opcodes.html")
rows = page.xpath("body/table")[0].findall("tr")
short_name_table = {}
for row in range(1, 17):
  for column in range(1, 17):
    ident = "0x" + str(format(row - 1, 'X')) + str(format(column - 1, 'X'))
    try:
      c = rows[row][column].text_content()
      s = c.split('\xa0')
      operation_name = s[0][:-1]
      flags = c[-7:]
      bytes_size = s[0][-1:]
      cycles_amount = s[2][:-7]
      str_cycles_amount = ', '.join(cycles_amount.split('/'))

      short_name = operation_name.split(' ')[0]

      if short_name_table.get(short_name) is None:
        short_name_table[short_name] = list()

      func = """// {} {}
// {}
auto {}_{} = []()""".format(ident, operation_name, flags, short_name, ident)
      func += """
{

};"""
      short_name_table.get(short_name).append(func)
    except:
      pass


for curr_file in short_name_table:
  with open(curr_file + ".h" ,"w+") as new_file:
    new_file.write("""/**
 * @file        {}.h
 * @author      Noam Rodrik
 * @description Implementation to the {} instruction(s).
 */""".format(curr_file, curr_file))
    new_file.write("""
#ifndef __LR35902_{}_H__
#define __LR35902_{}_H__

#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>

namespace Core""".format(curr_file, curr_file))
    new_file.write("\n{")
    short_name_table[curr_file] = [func + '\n' for func in short_name_table[curr_file]]
    short_name_table[curr_file][-1] = short_name_table[curr_file][-1][:-1]
    new_file.write('\n')
    new_file.write('\n'.join(short_name_table[curr_file]))
    new_file.write("\n} // Core\n")
    new_file.write('\n')
    new_file.write("""#endif // __LR35902_{}_H__""".format(curr_file))