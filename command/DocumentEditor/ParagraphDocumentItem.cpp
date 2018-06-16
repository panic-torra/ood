#include "stdafx.h"
#include "ParagraphDocumentItem.h"

CParagraphDocumentItem::CParagraphDocumentItem(std::string const& paragraph, IHistory& history)
	: m_paragraph(std::make_shared<CParagraph>(paragraph, history))
{
}

std::string CParagraphDocumentItem::GetDescription() const
{
	return (boost::format("Paragraph: %1%") % m_paragraph->GetText()).str();
}

std::shared_ptr<IParagraph> CParagraphDocumentItem::GetParagraph()
{
	return m_paragraph;
}

std::shared_ptr<const IParagraph> CParagraphDocumentItem::GetParagraph() const
{
	return m_paragraph;
}

void CParagraphDocumentItem::AcceptExporter(CDocumentHtmlExporter* exporter) const
{
	exporter->AddParagraph(m_paragraph);
}