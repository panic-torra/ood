#pragma once
#include "Shape.h"
#include "Vertex.h"

class CEllipse : public CShape
{
public:
	CEllipse(Vertex center, double radiusX, double radiusY);

	void Draw(ICanvas & canvas) final;

	virtual RectangleDouble GetFrame()const final;
	virtual void SetFrame(const RectangleDouble & rect) final;

	~CEllipse();
private:
	Vertex m_center = { 0, 0 };
	double m_radiusX = 0;
	double m_radiusY = 0;
	RectangleDouble m_frame = { 0, 0, 0, 0 };

	void CalculateFrame();
};