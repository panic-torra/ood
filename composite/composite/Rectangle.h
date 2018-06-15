#pragma once
#include "Shape.h"
#include "Vertex.h"

class CRectangle : public CShape
{
public:
	CRectangle(Vertex leftTop, Vertex rightBottom);

	void Draw(ICanvas & canvas) final;

	virtual RectangleDouble GetFrame()const final;
	virtual void SetFrame(const RectangleDouble & rect) final;

	~CRectangle();
private:
	Vertex m_leftTop;
	Vertex m_rightBottom;
	RectangleDouble m_frame = { 0, 0, 0, 0 };

	void CalculateFrame();
};