#include "stdafx.h"
#include "Shape.h"
#include "Color.h"

void CShape::SetColor(Color color)
{
	m_color = color;
}

Color CShape::GetColor() const
{
	return m_color;
}