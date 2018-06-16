#pragma once
#include "stdafx.h"
#include "../DocumentEditor/IDocument.h"

class CEditCommand
{
public:
	CEditCommand(IDocument& document, std::istream& in, std::ostream& out);

	void HandleCommand();
	void PrintHelp();
	void PrintDocument();

private:
	void PrintCarriage();
	void InsertParagraph(std::istream& args);
	void InsertImage(std::istream& args);
	void SetTitle(std::istream& args);
	void ReplaceText(std::istream& args);
	void ResizeImage(std::istream& args);
	void DeleteItem(std::istream& args);
	void Undo(std::istream& args);
	void Redo(std::istream& args);
	void Save(std::istream& args);
	std::string ReadLineFromRemainingInput(std::istream& in);
	boost::optional<size_t> ReadPositionFromInput(std::istream& in);

	typedef std::map<std::string, std::function<void(std::istream& args)>> ActionMap;

	const ActionMap m_actionMap;
	IDocument& m_document;
	std::istream& m_in;
	std::ostream& m_out;
};