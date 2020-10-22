/**
 * @file		Tools.h
 * @author		Noam Rodrik
 * @description Defines tools we can use across the emulator.
 */
#ifndef __TOOLS_H__
#define __TOOLS_H__

#include <stdint.h>
#include <stdio.h>

#define MacroStr(x)   #x
#define MacroStr2(x)  MacroStr(x)
#define Message(desc) __pragma(message(__FILE__ "(" MacroStr2(__LINE__) ") : " desc))

#define LOG_NO_ENTER(fmt, ...)				\
		do									\
		{									\
			printf(fmt, __VA_ARGS__);		\
		} while (false)

#define LOG(fmt, ...)							 \
		do										 \
		{										 \
			LOG_NO_ENTER(fmt "\n", __VA_ARGS__); \
		} while (false)

#define SANITY(cond, fmt, ...)				    \
		do									    \
		{									    \
			if (!(cond))					    \
			{								    \
				STOP_RUNNING(fmt, __VA_ARGS__); \
			}								    \
		} while (false)

#define STOP_RUNNING(fmt, ...)				   \
		do									   \
		{									   \
			LOG(fmt, __VA_ARGS__);		       \
			std::abort();					   \
		} while (false)

#define RET_FALSE_IF_FAIL(cond, fmt, ...)	   \
		do									   \
		{									   \
			if (!(cond))					   \
			{								   \
				LOG(fmt, __VA_ARGS__);         \
				return false;				   \
			}								   \
		} while (false)

namespace Tools
{
[[nodiscard]] static constexpr size_t Pow(size_t base, size_t power)
{
	return power == 0 ? 1 : Pow(base, power - 1) * base;
}

[[nodiscard]] static constexpr size_t SlotsToBytes(const uint32_t slots)
{
	return Pow(2, slots);
}

[[nodiscard]] static constexpr size_t MebibytesToBytes(size_t mebibytes)
{
	// 1 Mebibyte = 1024 Kibibyte
	// = 1024 * 1024 bytes
	return mebibytes * 1'024 * 1'024;
}

[[nodiscard]] static constexpr bool IsBitSet(const uint8_t data, const size_t index)
{
	return (data >> index) & 0x01;
}

[[nodiscard]] static constexpr void SetBit(uint8_t& data, const size_t index)
{
	data |= 1 << index;
}

[[nodiscard]] static constexpr void ClearBit(uint8_t& data, const size_t index)
{
	data &= static_cast<uint8_t>(~(1 << index));
}

[[nodiscard]] static constexpr void MutateBitByCondition(const bool condition, uint8_t& data, const size_t index)
{
	(condition ? SetBit(data, index) : ClearBit(data, index));
}

[[nodiscard]] static constexpr size_t BytesInROMBanks(const size_t bank_size)
{
	return bank_size * 0x4000;
}

[[nodiscard]] static constexpr size_t BytesInRAMBanks(const size_t bank_size)
{
	return bank_size * 0x2000;
}

[[nodiscard]] static constexpr uint8_t ReverseByte(const uint8_t byte)
{
	return ((byte & 0x1) << 7) |  ((byte & 0x2) << 5) |
		   ((byte & 0x4) << 3) |  ((byte & 0x8) << 1) |
		   ((byte & 0x10) >> 1) | ((byte & 0x20) >> 3) |
		   ((byte & 0x40) >> 5) | ((byte & 0x80) >> 7);
}
} // Tools

#endif // __TOOLS_H__