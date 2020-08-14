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

#define NO_PIXEL_ENGINE
#define TESTING

Message("If you want to print, comment this here, otherwise uncomment.");
#define NO_PRINT
#define NO_PRINT_FLAGS
#define NO_PRINT_REGISTERS
#define NO_PRINT_IF_AND_IE

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
			LOG(fmt, __VA_ARGS__);			   \
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


#if _DEBUG
#ifdef _MSC_BUILD
#include <Windows.h>
#include <iostream>
#include <sstream>

#define VISUAL_STUDIO_OUTPUT_VIEW_PRINT(character) \
		do{										   \
			std::wostringstream os_{};			   \
			os_ << static_cast<char>(character);   \
			OutputDebugStringW(os_.str().c_str()); \
		} while(false)

#define SECONDARY_OUTPUT(character) VISUAL_STUDIO_OUTPUT_VIEW_PRINT(character)
#else
#define SECONDARY_OUTPUT(character) LOG("%c", character)
#endif
#else
#define SECONDARY_OUTPUT(character) MAIN_LOG_NO_ENTER("%c", character)
#endif

namespace Tools
{
static constexpr uint32_t pow(uint32_t base, uint32_t power)
{
	return power == 0 ? 1 : pow(base, power - 1) * base;
}

static constexpr uint32_t KilobitsToBits(uint32_t kb)
{
	return pow(2, kb);
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
} // Tools

#endif // __TOOLS_H__