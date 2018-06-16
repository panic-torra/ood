#pragma once
#include "stdafx.h"

class CDocumentTempPath
{
public:
	~CDocumentTempPath();
	boost::filesystem::path GetTempPath() const;

private:
	mutable boost::optional<boost::filesystem::path> m_tempPath;
};