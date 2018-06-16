#include "stdafx.h"
#include "Image.h"
#include "ChangeItemCommand.h"

CImage::CImage(IFileResource::Ptr && resource, ImageSize const& size, IHistory& history)
	: m_resource(resource)
	, m_size(size)
	, m_history(history)
{
}

void CImage::Resize(unsigned width, unsigned height)
{
	m_history.Push(std::make_unique<CChangeItemCommand<ImageSize>>(m_size, ImageSize(width, height)));
}

boost::filesystem::path CImage::GetPath()const
{
	return m_resource->GetPath();
}

unsigned CImage::GetWidth()const
{
	return m_size.width;
}

unsigned CImage::GetHeight()const
{
	return m_size.height;
}

bool CImage::operator==(CImage const& other) const
{
	return m_resource->GetPath() == other.m_resource->GetPath()
		&& m_size.height == other.m_size.height
		&& m_size.width == other.m_size.width;
}