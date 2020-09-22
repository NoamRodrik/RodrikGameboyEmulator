/**
 * @file		IPPU.h
 * @author		Noam Rodrik
 * @description The interface of the PPU.
 */
#ifndef __CORE_IPPU_H__
#define __CORE_IPPU_H__

#include <cstddef>

namespace Core
{
class IPPU
{
public:
	virtual ~IPPU() = default;

public:
	virtual bool Startup() = 0;
	virtual void Clock(std::size_t clock) = 0;
};
}

#endif // __CORE_IPPU_H__