#pragma once
#include "Shape.h"
#include "ICanvas.h"
#include "Color.h"

class CTriangle : public CShape
{
public:
	CTriangle(Vertex const & vertex1, Vertex const & vertex2, Vertex const & vertex3, Color const & color);

	void Draw(ICanvas & canvas) const override;

	Vertex GetVertex1() const;
	Vertex GetVertex2() const;
	Vertex GetVertex3() const;

private:
	Vertex m_vertex1;
	Vertex m_vertex2;
	Vertex m_vertex3;
};
