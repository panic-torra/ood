#include "stdafx.h"
#include "DocumentItem.h"

std::shared_ptr<IImage> CDocumentItem::GetImage()
{
	return std::shared_ptr<IImage>();
}

std::shared_ptr<const IImage> CDocumentItem::GetImage() const
{
	return std::shared_ptr<const IImage>();
}

std::shared_ptr<IParagraph> CDocumentItem::GetParagraph()
{
	return std::shared_ptr<IParagraph>();
}

std::shared_ptr<const IParagraph> CDocumentItem::GetParagraph() const
{
	return std::shared_ptr<const IParagraph>();
}
