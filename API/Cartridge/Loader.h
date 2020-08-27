/**
 * @file		Loader.h
 * @author		Noam Rodrik
 * @description Serves as a service to load from the hard-disk.
 */
#ifndef __API_LOADER_H__
#define __API_LOADER_H__

#include <Contrib/GSL/not_null.h>
#include <API/Definitions.h>
#include <Tools/Tools.h>
#include "ILoader.h"
#include <string>

namespace API
{
class Loader : public ILoader
{
public:
	Loader(const std::string& binary_path) : m_binary_path{binary_path}, m_binary_file{fopen(binary_path.c_str(), FILE_READ_MODE)}
	{
	}

	virtual ~Loader()
	{
		SANITY(fclose(this->m_binary_file) == 0, "Failed closing game loader file ptr");
	}

	virtual void Load(API::data_t* const destination, const long size, const long offset = 0) override
	{
		SANITY(fseek(this->m_binary_file, offset, SEEK_SET) == 0, "Failed seeking!");
		SANITY(fread(destination, 1, size, this->m_binary_file) <= size, "Failed reading from file!");
	}

private:
	static constexpr const char FILE_READ_MODE[] = "rb";

private:
	const std::string&   m_binary_path;
	gsl::not_null<FILE*> m_binary_file;
};
} // API

#endif // __API_LOADER_H__