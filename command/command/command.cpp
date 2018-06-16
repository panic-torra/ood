#include "stdafx.h"
#include "../DocumentEditor/Document.h"
#include "EditCommand.h"

int main()
{
	CDocument document;
	CEditCommand editor(document, std::cin, std::cout);

	while (!std::cin.eof() && !std::cin.fail())
	{
		editor.HandleCommand();
	}
}