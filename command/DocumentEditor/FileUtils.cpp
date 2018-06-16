#include "stdafx.h"
#include "FileUtils.h"

using namespace std;
using namespace std::chrono;
using namespace boost::filesystem;

namespace FileUtils
{
	void CopyFile(path const& from, path const& to)
	{
		try
		{
			copy(from, to);
		}
		catch (filesystem_error const&)
		{
			throw runtime_error("Failed to make file copy");
		}
	}

	void Delete(path const& path)
	{
		try
		{
			remove_all(path);
		}
		catch (filesystem_error const&)
		{
			throw runtime_error("Failed to remove file");
		}
	}

	void CreateDirReqursively(path const& dirPath)
	{
		try
		{
			create_directories(path(dirPath));
		}
		catch (filesystem_error const&)
		{
			throw runtime_error("Failed to create directories");
		}
	}

	bool Exists(path const& path)
	{
		return exists(path);
	}

	std::string GenerateUniqueFileName(path const& oldName)
	{
		auto extension = boost::filesystem::extension(oldName);
		auto timestamp = (system_clock::now()).time_since_epoch();
		return to_string(duration_cast<milliseconds>(timestamp).count()) + extension;
	}
}