/**
 * @file		Memory.h
 * @author		Noam Rodrik
 * @description LR35902 main memory definition header.
 */
#ifndef __LR35902_MEMORY_H__
#define __LR35902_MEMORY_H__

#include <Core/API/Memory/MemoryBase.h>

namespace Core
{
template <size_t SIZE>
class Memory : public MemoryBase<SIZE>
{
public:
	using MemoryBase<SIZE>::MemoryBase;
};
} // Core

#endif // __LR35902_MEMORY_H__
