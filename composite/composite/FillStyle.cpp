#include "stdafx.h"
#include "FillStyle.h"

CFillStyle::CFillStyle()
{
}


CFillStyle::~CFillStyle()
{
}

std::shared_ptr<IStyle> CFillStyle::GetStyle(std::shared_ptr<IShape> shape)
{
	return shape->GetFillStyle();
}
