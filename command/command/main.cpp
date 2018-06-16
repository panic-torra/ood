#include "stdafx.h"
#include "EditCommand.h"
#include "../DocumentEditor/Document.h"

int main()
{
	CDocument document;
	CEditCommand editor(document, std::cin, std::cout);

	while (!std::cin.eof() && !std::cin.fail())
	{
		editor.HandleCommand();
	}
}