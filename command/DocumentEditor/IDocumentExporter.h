#pragma once
#include "stdafx.h"
#include "IParagraph.h"
#include "IIMage.h"

class IDocumentExporter
{
public:
	virtual void AddParagraph(std::shared_ptr<IParagraph> const& paragraph) = 0;
	virtual void AddImage(std::shared_ptr<IImage> const& image) = 0;

	virtual ~IDocumentExporter() = default;
};