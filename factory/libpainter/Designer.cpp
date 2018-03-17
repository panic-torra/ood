#include "stdafx.h"
#include "Designer.h"
#include "PictureDraft.h"

CDesigner::CDesigner(IShapeFactory & factory)
	:m_factory(factory)
{
}

CPictureDraft CDesigner::CreateDraft(std::istream & input)
{
	CPictureDraft draft;
	std::string line;
	while (std::getline(input, line))
	{
		try
		{
			draft.AddShape(m_factory.CreateShape(line));
		}
		catch (std::exception & e)
		{
			std::cout << e.what() << std::endl;
		}
	}

	return draft;
}
