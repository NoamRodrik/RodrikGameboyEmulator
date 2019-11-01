/**
 * @file		StaticInstance.h
 * @author		Noam Rodrik
 * @description Base class for classes that need static instances.
 */
#ifndef __STATIC_INSTANCE_H__
#define __STATIC_INSTANCE_H__

#include <Tools/not_null.h>

namespace Core
{
template <typename T>
class StaticInstance
{
public:
	static T& GetInstance()
	{
		if (typename T::Instance == nullptr)
		{
			Instance = gsl::not_null<T*>(new T{});
		}

		return *gsl::not_null<T*>(typename T::Instance);
	}

private:
	inline static T* Instance{nullptr};
};
} // Core

#endif // __STATIC_INSTANCE_H__