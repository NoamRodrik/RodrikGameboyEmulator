/**
 * @file		StaticInstance.h
 * @author		Noam Rodrik
 * @description Base class for classes that need static instances.
 */
#ifndef __STATIC_INSTANCE_H__
#define __STATIC_INSTANCE_H__

#include <Tools/not_null.h>

namespace API
{
template <typename T>
class StaticInstance
{
public:
	static T& GetInstance()
	{
		if (Instance == nullptr)
		{
			Instance = gsl::not_null<T*>(new T{});
		}

		return *gsl::not_null<T*>(Instance);
	}

private:
	inline static T* Instance{nullptr};
};
} // Core

#endif // __STATIC_INSTANCE_H__