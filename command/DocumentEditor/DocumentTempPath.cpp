#include "stdafx.h"
#include "DocumentTempPath.h"
#include "FileUtils.h"

CDocumentTempPath::~CDocumentTempPath()
{
	if (m_tempPath != boost::none)
	{
		try
		{
			FileUtils::Delete(*m_tempPath);
		}
		catch (...)
		{
		}
	}
}

boost::filesystem::path CDocumentTempPath::GetTempPath() const
{
	if (m_tempPath == boost::none)
	{
		boost::filesystem::path tempPath = boost::filesystem::temp_directory_path() / boost::filesystem::unique_path("document-%%%%%%");
		FileUtils::CreateDirReqursively(tempPath);
		m_tempPath = tempPath;
	}
	return (*m_tempPath);
}