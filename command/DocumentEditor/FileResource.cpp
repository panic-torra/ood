#include "stdafx.h"
#include "FileResource.h"
#include "FileUtils.h"

CFileResource::CFileResource(boost::filesystem::path filePath)
	: m_filePath(filePath)
{
}

CFileResource::~CFileResource()
{
	try
	{
		FileUtils::Delete(m_filePath);
	}
	catch (std::runtime_error const&)
	{
	}
}

boost::filesystem::path CFileResource::GetPath()const
{
	return m_filePath;
}
