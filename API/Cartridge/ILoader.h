#ifndef __API_ILOADER_H__
#define __API_ILOADER_H__

#include <API/Definitions.h>

namespace API
{
class ILoader
{
public:
	virtual ~ILoader() = default;

public:
	virtual void Load(API::data_t* const destination, const long size, const long offset = 0) = 0;
};
} // API

#endif // __API_ILOADER_H__