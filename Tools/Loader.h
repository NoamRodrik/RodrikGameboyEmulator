/**
 * @file		Loader.h
 * @author		Noam Rodrik
 * @description Serves as a service to load from the hard-disk.
 */
#ifndef __TOOLS_LOADER_H__
#define __TOOLS_LOADER_H__

#include <API/Cartridge/ILoader.h>
#include <Contrib/GSL/not_null.h>
#include <API/Definitions.h>
#include <Tools/Tools.h>
#include <string>

namespace Tools
{
class [[nodiscard]] Loader : public API::ILoader
{
public:
	explicit Loader(const std::string& binary_path) : _binary_path{binary_path}, _binary_file{fopen(binary_path.c_str(), FILE_READ_MODE)}
	{
	}

	virtual ~Loader() override
	{
		SANITY(fclose(this->_binary_file) == 0, "Failed closing game loader file ptr");
	}

	virtual void Load(API::data_t* const destination, const long size, const long offset = 0) override
	{
		SANITY(fseek(this->_binary_file, offset, SEEK_SET) == 0, "Failed seeking!");
		SANITY(fread(destination, 1, size, this->_binary_file) <= size, "Failed reading from file!");
	}

private:
	static constexpr const char FILE_READ_MODE[] = "rb";

private:
	const std::string&   _binary_path;
	gsl::not_null<FILE*> _binary_file;
};
} // Tools

#endif // __TOOLS_LOADER_H__