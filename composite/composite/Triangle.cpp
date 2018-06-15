#include "stdafx.h"
#include "Triangle.h"

using namespace std;

CTriangle::CTriangle(Vertex one, Vertex two, Vertex three)
	: m_one(one)
	, m_two(two)
	, m_three(three)
{
	CalculateFrame();
}


CTriangle::~CTriangle()
{
}


void CTriangle::Draw(ICanvas & canvas)
{
	SetCanvasStyles(canvas);
	canvas.MoveTo(m_one.x, m_one.y);
	canvas.LineTo(m_two.x, m_two.y);
	canvas.LineTo(m_three.x, m_three.y);
	canvas.LineTo(m_one.x, m_one.y);
}

RectangleDouble CTriangle::GetFrame()const
{
	return m_frame;
}

void CTriangle::SetFrame(const RectangleDouble & rect)
{
	RectangleDouble oldFrame = GetFrame();

	CalculateNewVertexPosition(m_one, oldFrame, rect);
	CalculateNewVertexPosition(m_two, oldFrame, rect);
	CalculateNewVertexPosition(m_three, oldFrame, rect);

	CalculateFrame();
}

void CTriangle::CalculateFrame()
{
	double minX = min(m_one.x, m_two.x);
	minX = min(minX, m_three.x);

	double minY = min(m_one.y, m_two.y);
	minY = min(minY, m_three.y);

	double maxX = max(m_one.x, m_two.x);
	maxX = max(maxX, m_three.x);

	double maxY = max(m_one.y, m_two.y);
	maxY = max(maxY, m_three.y);

	m_frame = { minX , minY, maxX - minX, maxY - minY };
}

void CTriangle::CalculateNewVertexPosition(Vertex &vertex, RectangleDouble oldFrame, RectangleDouble newFrame)
{
	vertex.x = newFrame.left + (vertex.x - oldFrame.left) / (oldFrame.width / newFrame.width);
	vertex.y = newFrame.top + (vertex.y - oldFrame.top) / (oldFrame.height / newFrame.height);
}