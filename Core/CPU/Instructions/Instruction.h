/**
 * @file		Instruction.h
 * @author		Noam Rodrik
 * @description A class that represents how instructions look and act.
 */
#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

#include <Core/CPU/Processor.h>
#include <functional>
#include <stdint.h>

namespace Core
{
#pragma pack(push, 1)
class Instruction
{
public:
	[[nodiscard]] inline bool Execute() const { return this->_func(); }

public:
	const std::function<bool()> _func{nullptr};
	const uint8_t				bytes_size{0};
	const uint8_t				cycles_amount{0};
	const uint8_t				extended_cycles_amount{cycles_amount};
	const std::string			operation_string;
};
#pragma pack(pop)
} // Core

#endif // __INSTRUCTION_H__