#include "stdafx.h"
#include "OutlineStyle.h"

COutlineStyle::COutlineStyle()
{
}


COutlineStyle::~COutlineStyle()
{
}

std::shared_ptr<IStyle> COutlineStyle::GetStyle(std::shared_ptr<IShape> shape)
{
	return shape->GetOutlineStyle();
}