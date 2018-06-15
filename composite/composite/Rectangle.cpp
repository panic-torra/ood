#include "stdafx.h"
#include "Rectangle.h"

CRectangle::CRectangle(Vertex leftTop, Vertex rightBottom)
	: m_leftTop(leftTop)
	, m_rightBottom(rightBottom)
{
	CalculateFrame();
}


CRectangle::~CRectangle()
{
}

void CRectangle::Draw(ICanvas & canvas)
{
	SetCanvasStyles(canvas);
	canvas.MoveTo(m_leftTop.x, m_leftTop.y);
	canvas.LineTo(m_rightBottom.x, m_leftTop.y);
	canvas.LineTo(m_rightBottom.x, m_rightBottom.y);
	canvas.LineTo(m_leftTop.x, m_rightBottom.y);
	canvas.LineTo(m_leftTop.x, m_leftTop.y);
}

RectangleDouble CRectangle::GetFrame()const
{
	return m_frame;
}

void CRectangle::SetFrame(const RectangleDouble & rect)
{
	m_leftTop = { rect.left, rect.top };
	m_rightBottom = { m_leftTop.x + rect.width, m_leftTop.y + rect.height };
	CalculateFrame();
}

void CRectangle::CalculateFrame()
{
	double width = m_rightBottom.x - m_leftTop.x;
	double height = m_rightBottom.y - m_leftTop.y;

	m_frame = { m_leftTop.x, m_leftTop.y, width, height };
}
