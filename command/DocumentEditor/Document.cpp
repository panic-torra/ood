#include "stdafx.h"
#include "Document.h"
#include "ParagraphDocumentItem.h"
#include "ImageDocumentItem.h"
#include "InsertItemCommand.h"
#include "ChangeItemCommand.h"
#include "DeleteItemCommand.h"
#include "FileResource.h"
#include "FileUtils.h"
#include "DocumentExporter.h"

CDocument::CDocument()
	: m_title("Untitled document")
{
}

std::shared_ptr<IParagraph> CDocument::InsertParagraph(std::string const & text, boost::optional<size_t> position)
{
	ValidateInsertPosition(position);

	std::shared_ptr<CParagraphDocumentItem> item = std::make_shared<CParagraphDocumentItem>(text, m_history);

	std::unique_ptr<CInsertItemCommand<std::list<IDocumentItem::Ptr>>> command = std::make_unique<CInsertItemCommand<std::list<IDocumentItem::Ptr>>>(m_items, item, position);
	m_history.Push(std::move(command));

	return item->GetParagraph();
}

std::shared_ptr<IImage> CDocument::InsertImage(boost::filesystem::path const & path, unsigned width, unsigned height, boost::optional<size_t> position)
{
	ValidateInsertPosition(position);

	IFileResource::Ptr fileResource = GetCopiedImageResource(path);
	std::shared_ptr<CImageDocumentItem> item = std::make_shared<CImageDocumentItem>(std::move(fileResource), ImageSize(width, height), m_history);

	std::unique_ptr<CInsertItemCommand<std::list<IDocumentItem::Ptr>>> command = std::make_unique<CInsertItemCommand<std::list<IDocumentItem::Ptr>>>(m_items, item, position);
	m_history.Push(std::move(command));

	return item->GetImage();
}

size_t CDocument::GetItemsCount() const
{
	return m_items.size();
}

IDocumentItem::Ptr CDocument::GetItem(size_t index)
{
	ValidateItemPosition(index);

	auto it = m_items.begin();
	std::advance(it, index);
	return *it;
}

IDocumentItem::ConstPtr CDocument::GetItem(size_t index) const
{
	ValidateItemPosition(index);

	auto it = m_items.begin();
	std::advance(it, index);
	return *it;
}

void CDocument::DeleteItem(size_t index)
{
	ValidateItemPosition(index);

	m_history.Push(std::make_unique<CDeleteItemCommand<std::list<IDocumentItem::Ptr>>>(m_items, index));
}

void CDocument::SetTitle(std::string const& title)
{
	if (title.empty())
	{
		throw std::runtime_error("Can't set an empty title!");
	}

	m_history.Push(std::make_unique<CChangeItemCommand<std::string>>(m_title, title));
}

std::string CDocument::GetTitle() const
{
	return m_title;
}

bool CDocument::CanUndo() const
{
	return m_history.CanUndo();
}

void CDocument::Undo()
{
	m_history.Undo();
}

bool CDocument::CanRedo() const
{
	return m_history.CanRedo();
}

void CDocument::Redo()
{
	m_history.Redo();
}

void CDocument::Save(boost::filesystem::path const & path) const
{
	std::unique_ptr<CDocumentHtmlExporter> htmlExporter = std::make_unique<CDocumentHtmlExporter>(*this, path);
	std::for_each(m_items.begin(), m_items.end(), [&htmlExporter](IDocumentItem::ConstPtr const& item) {
		item->AcceptExporter(htmlExporter.get());
	});
}

IFileResource::Ptr CDocument::GetCopiedImageResource(boost::filesystem::path source)
{
	try
	{
		boost::filesystem::path resourcePath = m_resourcePath.GetTempPath() / FileUtils::GenerateUniqueFileName(source);
		FileUtils::CopyFile(source, resourcePath);
		return std::static_pointer_cast<IFileResource>(std::make_shared<CFileResource>(resourcePath));
	}
	catch (...)
	{
		throw std::runtime_error("Failed to copy resource file!");
	}
}

void CDocument::ValidateInsertPosition(boost::optional<size_t> position) const
{
	if (position == boost::none)
	{
		return;
	}

	const size_t itemsCount = m_items.size();
	if ((*position) > itemsCount)
	{
		throw std::runtime_error("Invalid insert position!");
	}
}

void CDocument::ValidateItemPosition(size_t position) const
{
	const size_t itemsCount = m_items.size();
	if (itemsCount == 0 || position > itemsCount - 1)
	{
		throw std::runtime_error("Invalid item position!");
	}
}

boost::filesystem::path CDocument::GetTempPath() const
{
	return m_resourcePath.GetTempPath();
}