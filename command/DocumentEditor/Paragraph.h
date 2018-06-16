#pragma once
#include "IParagraph.h"
#include "IHistory.h"

class CParagraph : public IParagraph
{
public:
	CParagraph(std::string const& paragraph, IHistory& history);
	std::string GetText() const override;
	void SetText(std::string const & text) override;

	bool operator==(CParagraph const& other) const;

private:
	std::string m_paragraph;
	IHistory& m_history;
};