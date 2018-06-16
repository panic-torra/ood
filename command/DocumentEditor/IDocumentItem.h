#pragma once
#include "stdafx.h"
#include "IIMage.h"
#include "IParagraph.h"
#include "DocumentExporter.h"

class IDocumentItem
{
public:
	typedef std::shared_ptr<IDocumentItem> Ptr;
	typedef std::shared_ptr<const IDocumentItem> ConstPtr;

	virtual std::shared_ptr<IImage> GetImage() = 0;
	virtual std::shared_ptr<const IImage> GetImage()const = 0;

	virtual std::shared_ptr<IParagraph> GetParagraph() = 0;
	virtual std::shared_ptr<const IParagraph> GetParagraph()const = 0;

	virtual std::string GetDescription()const = 0;
	virtual void AcceptExporter(CDocumentHtmlExporter* exporter)const = 0;

	virtual ~IDocumentItem() = default;
};