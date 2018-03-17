#pragma once
#include <iosfwd>
#include "IDesigner.h"
#include "IShapeFactory.h"
#include "ShapeFactory.h"

class CDesigner : public IDesigner
{
public:
	CDesigner(IShapeFactory & factory);
	CPictureDraft CreateDraft(std::istream & input);

private:
	IShapeFactory & m_factory;
};

