#pragma once
#include "IFileResource.h"

class CFileResource : public IFileResource
{
public:
	CFileResource(boost::filesystem::path filePath);
	~CFileResource();

	boost::filesystem::path GetPath()const override;

private:
	boost::filesystem::path m_filePath;
};