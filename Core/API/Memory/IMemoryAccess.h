/**
 * @file		IMemoryAccess.h
 * @author		Noam Rodrik
 * @description Defines basic actions for memory access objects.
 */
#ifndef __IMEMORY_ACCESS_H__
#define __IMEMORY_ACCESS_H__

#include <Core/API/Definitions.h>

namespace Core
{
class IMemoryAccess
{
public:
	virtual data_t& operator[](const address_t address) = 0;
	virtual const data_t& operator[](const address_t address) const = 0;
};
} // Core

#endif // __IMEMORY_ACCESS_H__