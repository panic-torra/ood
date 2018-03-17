#pragma once
#include "Shape.h"

class CRectangle :public CShape
{
public:
	CRectangle(Vertex const & leftTop, Vertex const & rightBottom, Color const & color);

	Vertex GetLeftTop() const;
	Vertex GetRightBottom() const;

	void Draw(ICanvas &canvas) const override;

private:
	Vertex m_leftTop = { (float) 0.5, (float) 0.5 };
	Vertex m_rightBottom = { 0, 0 };
};
