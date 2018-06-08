#pragma once
#include "Color.h"
#include "Vertex.h"

class ICanvas
{
public:
	virtual void SetColor(Color color) = 0;
	virtual void DrawLine(const Vertex from, const Vertex to) = 0;
	virtual void DrawEllipse(const Vertex center, const float width, const float height) = 0;

	virtual ~ICanvas() = default;
};