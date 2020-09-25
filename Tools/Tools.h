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

#define SERIAL_TRANSFER_PRINT_DEBUG
//#define FULL_PALETTE_TEST

Message("If you want to print, comment this here, otherwise uncomment.");
//#define NO_PRINT
#define NO_PRINT_FLAGS
#define NO_PRINT_REGISTERS
#define NO_PRINT_IF_AND_IE
//#define NO_PRINT_COMMANDS
//#define PRINT_ONLY_PC

#ifndef NO_PRINT
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
#else
#define LOG_NO_ENTER(fmt, ...)
#define LOG(fmt, ...)	
#endif

#define MAIN_LOG_NO_ENTER(fmt, ...)	  \
		do							  \
		{							  \
			printf(fmt, __VA_ARGS__); \
		} while (false)

#define MAIN_LOG(fmt, ...) MAIN_LOG_NO_ENTER(fmt "\n", __VA_ARGS__)

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
			MAIN_LOG(fmt, __VA_ARGS__);		   \
			std::abort();					   \
		} while (false)

#define RET_FALSE_IF_FAIL(cond, fmt, ...)	   \
		do									   \
		{									   \
			if (!(cond))					   \
			{								   \
				MAIN_LOG(fmt, __VA_ARGS__);    \
				return false;				   \
			}								   \
		} while (false)

namespace Tools
{
static constexpr size_t Pow(size_t base, size_t power)
{
	return power == 0 ? 1 : Pow(base, power - 1) * base;
}

static constexpr size_t SlotsToBytes(const uint32_t slots)
{
	return Pow(2, slots);
}

static constexpr size_t MebibytesToBytes(size_t mebibytes)
{
	// 1 Mebibyte = 1024 Kibibyte
	// = 1024 * 1024 bytes
	return mebibytes * 1'024 * 1'024;
}

static constexpr bool IsBitSet(const uint8_t data, const size_t index)
{
	return (data >> index) & 0x01;
}

static constexpr void SetBit(uint8_t& data, const size_t index)
{
	data |= 1 << index;
}

static constexpr void ClearBit(uint8_t& data, const size_t index)
{
	data &= static_cast<uint8_t>(~(1 << index));
}

static constexpr void MutateBitByCondition(const bool condition, uint8_t& data, const size_t index)
{
	(condition ? SetBit(data, index) : ClearBit(data, index));
}

static constexpr size_t BytesInROMBanks(const size_t bank_size)
{
	return bank_size * 0x4000;
}

static constexpr size_t BytesInRAMBanks(const size_t bank_size)
{
	return bank_size * 0x2000;
}
} // Tools

#endif // __TOOLS_H__