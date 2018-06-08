#pragma once
#include "Shape.h"

class CRegularPolygon : public CShape
{
public:
	CRegularPolygon(Vertex const & center, float radius, unsigned vertexCount, Color const color);

	void Draw(ICanvas& canvas) const override;
	Vertex GetCenter() const;
	float GetRadius() const;
	int GetVertexCount() const;
	std::vector<Vertex> GetVerticies() const;

private:
	Vertex m_center;
	float m_radius;
	unsigned m_vertexCount;
};

