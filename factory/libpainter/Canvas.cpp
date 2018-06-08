#include "stdafx.h"
#include "Canvas.h"
#include "ColorUtils.h";

void CCanvas::SetColor(Color color)
{
	m_color = color;
	m_output << "Color: " << GetColorParamsHelper::GetColorStringValue(color) << std::endl;
}

void CCanvas::DrawLine(const Vertex from, const Vertex to)
{
	m_output << "Line: (" << from.x << ", " << from.y << "), (" << to.x << ", " << to.y << ")" << std::endl;
}

void CCanvas::DrawEllipse(const Vertex center, float width, float height)
{
	m_output << "Ellipse: (" << center.x << ", " << center.y << "), Width: " << width << ", Height: " << height << std::endl;
}