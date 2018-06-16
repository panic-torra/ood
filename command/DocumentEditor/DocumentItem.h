#pragma once
#include "IDocumentItem.h"

class CDocumentItem : public IDocumentItem
{
public:
	std::shared_ptr<IImage> GetImage() override;
	std::shared_ptr<const IImage> GetImage() const override;
	std::shared_ptr<IParagraph> GetParagraph() override;
	std::shared_ptr<const IParagraph> GetParagraph() const override;
};