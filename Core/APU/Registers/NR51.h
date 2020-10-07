/**
 * @file		NR51.h
 * @author		Noam Rodrik
 * @description LR35902's main apu NR51 definition header.
 */
#ifndef __LR35902_NR51_H__
#define __LR35902_NR51_H__

#include <Core/CPU/Registers/MemoryRegister.h>

namespace Core
{
class NR51 : public MemoryRegister<API::data_t>
{
public:
	constexpr NR51() : MemoryRegister{NR51_ADDRESS} {}
	NR51(const API::data_t value) : MemoryRegister{NR51_ADDRESS} { *this = value; }

public:
	using MemoryRegister::operator=;
	using MemoryRegister::operator API::data_t;

public:
	static constexpr std::size_t OutputTerminalOffset(const OutputTerminal output)
	{
		switch (output)
		{
			case OutputTerminal::SO1:
			{
				return NR51::SO1_OUTPUT_TERMINAL_OFFSET;
			}

			case OutputTerminal::SO2:
			{
				return NR51::SO2_OUTPUT_TERMINAL_OFFSET;
			}

			default:
			{
				return 0;
			}
		}
	}

public:
	static constexpr API::address_t NR51_ADDRESS{0xFF25};
	static constexpr API::address_t NR51_DEFAULT_VALUE{0xF3};

private:
	static constexpr std::size_t SO1_OUTPUT_TERMINAL_OFFSET{0x00};
	static constexpr std::size_t SO2_OUTPUT_TERMINAL_OFFSET{0x04};
};
} // Core

#endif // __LR35902_NR51_H__