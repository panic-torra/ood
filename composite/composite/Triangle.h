#pragma once
#include "Shape.h"
#include "Vertex.h"

class CTriangle : public CShape
{
public:
	CTriangle(Vertex one, Vertex two, Vertex three);

	void Draw(ICanvas & canvas) final;

	virtual RectangleDouble GetFrame()const final;
	virtual void SetFrame(const RectangleDouble & rect) final;

	~CTriangle();
private:
	Vertex m_one = { 0, 0 };
	Vertex m_two = { 0, 0 };
	Vertex m_three = { 0, 0 };
	RectangleDouble m_frame = { 0, 0, 0, 0 };

	void CalculateFrame();
	void CalculateNewVertexPosition(Vertex &vertex, RectangleDouble oldFrame, RectangleDouble newFrame);
};