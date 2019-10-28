/**
 * @file		BinaryLoader.h
 * @author		Noam Rodrik
 * @description Serves as a service to load binary from the hard-disk.
 */
#ifndef __BINARY_LOADER_H__
#define __BINARY_LOADER_H__

#include <Core/Processor/Processor.h>
#include <Tools/not_null.h>
#include <string>

namespace Core
{
class BinaryLoader
{
public:
	BinaryLoader(const std::string& binary_path, data_t* const destination, const long size) : m_binary_path{binary_path}, m_binary_file{fopen(binary_path.c_str(), FILE_READ_MODE)}
	{
		auto file_size = this->GetFileSize();

		// We might have a file size larger than the capable RAM size.
		if (file_size >= size)
		{
			file_size = size;
		}

		SANITY(fread(destination, 1, file_size, this->m_binary_file) == file_size, "Failed reading from file!");
	}

	virtual ~BinaryLoader()
	{
		SANITY(fclose(this->m_binary_file) == 0, "Failed closing game loader file ptr");
	}

	const long GetFileSize() const
	{
		// Seek to end of file
		SANITY(fseek(this->m_binary_file, 0, SEEK_END) == 0, "Failed seeking!");

		// get current file pointer
		auto size = ftell(m_binary_file);

		// seek back to beginning of file
		SANITY(fseek(m_binary_file, 0, SEEK_SET) == 0, "Failed seeking!");

		return size;
	}

private:
	static constexpr const char FILE_READ_MODE[] = "rb";

private:
	const std::string&   m_binary_path;
	gsl::not_null<FILE*> m_binary_file;
};
} // Core

#endif // __BINARY_LOADER_H__