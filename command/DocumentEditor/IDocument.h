#pragma once
#include "stdafx.h"
#include "IDocumentItem.h"
#include "IParagraph.h"
#include "IImage.h"

class IDocument
{
public:
	virtual std::shared_ptr<IParagraph> InsertParagraph(std::string const& text, boost::optional<size_t> position = boost::none) = 0;
	virtual std::shared_ptr<IImage> InsertImage(boost::filesystem::path const& path, unsigned width, unsigned height, boost::optional<size_t> position = boost::none) = 0;

	virtual size_t GetItemsCount()const = 0;

	virtual IDocumentItem::Ptr GetItem(size_t index) = 0;
	virtual IDocumentItem::ConstPtr GetItem(size_t index)const = 0;

	virtual void DeleteItem(size_t index) = 0;

	virtual void SetTitle(std::string const& title) = 0;
	virtual std::string GetTitle()const = 0;

	virtual bool CanUndo()const = 0;
	virtual void Undo() = 0;
	virtual bool CanRedo()const = 0;
	virtual void Redo() = 0;

	virtual void Save(boost::filesystem::path const& path)const = 0;

	virtual ~IDocument() = default;
};
