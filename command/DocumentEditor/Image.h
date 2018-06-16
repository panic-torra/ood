#pragma once
#include "IIMage.h"
#include "IDocumentItem.h"
#include "FileResource.h"
#include "IHistory.h"

class CImage : public IImage
{
public:
	CImage(IFileResource::Ptr && resource, ImageSize const& size, IHistory& history);

	void Resize(unsigned width, unsigned height) override;
	boost::filesystem::path GetPath()const override;
	unsigned GetWidth()const override;
	unsigned GetHeight()const override;

	bool operator==(CImage const& other) const;

private:
	ImageSize m_size;
	IFileResource::Ptr m_resource;
	IHistory& m_history;
};