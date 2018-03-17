#pragma once
#include "stdafx.h"
#include "Painter.h"

void CPainter::DrawPicture(CPictureDraft & draft, ICanvas &canvas)
{
	for (auto & shape : draft)
	{
		shape.Draw(canvas);
	}
}