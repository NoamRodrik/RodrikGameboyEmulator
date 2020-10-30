/**
 * @file		NR51.h
 * @author		Noam Rodrik
 * @description LR35902's main apu NR51 definition header.
 */
#ifndef __LR35902_NR51_H__
#define __LR35902_NR51_H__

#include <Core/CPU/Registers/DeviceRegister.h>

namespace Core
{
class [[nodiscard]] NR51 : public DeviceRegister<0xFF25>
{
public:
	using DeviceRegister::DeviceRegister;
	using DeviceRegister::operator=;
	using DeviceRegister::operator API::data_t;

public:
	[[nodiscard]] static constexpr std::size_t OutputTerminalOffset(const OutputTerminal output)
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
	static constexpr API::data_t NR51_DEFAULT_VALUE{0xF3};

private:
	static constexpr std::size_t SO1_OUTPUT_TERMINAL_OFFSET{0x00};
	static constexpr std::size_t SO2_OUTPUT_TERMINAL_OFFSET{0x04};
};
} // Core

#endif // __LR35902_NR51_H__