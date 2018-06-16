#pragma once
#include "DocumentItem.h"
#include "Image.h"
#include "IHistory.h"

class CImageDocumentItem : public CDocumentItem
{
public:
	CImageDocumentItem(IFileResource::Ptr && resource, ImageSize const& imageSize, IHistory& history);
	~CImageDocumentItem() = default;
	std::string GetDescription()const override;
	std::shared_ptr<IImage> GetImage() override;
	std::shared_ptr<const IImage> GetImage() const override;
	void AcceptExporter(CDocumentHtmlExporter* exporter) const override;

private:
	std::shared_ptr<CImage> m_image;
};