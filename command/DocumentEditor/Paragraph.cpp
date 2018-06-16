#include "stdafx.h"
#include "Paragraph.h"
#include "InsertItemCommand.h"
#include "ChangeItemCommand.h"

CParagraph::CParagraph(std::string const& paragraph, IHistory& history)
	: m_paragraph(paragraph), m_history(history)
{
}

std::string CParagraph::GetText() const
{
	return m_paragraph;
}

void CParagraph::SetText(std::string const& text)
{
	if (m_paragraph != text)
	{
		m_history.Push(std::make_unique<CChangeItemCommand<std::string>>(m_paragraph, text));
	}
}

bool CParagraph::operator==(CParagraph const& other) const
{
	return m_paragraph == other.m_paragraph;
}