#include "stdafx.h"
#include "Slide.h"

double CSlide::GetWidth() const
{
	if (m_shapes.GetShapesCount() == 0)
	{
		return 0.0;
	}

	RectangleDouble firstFrame = m_shapes.GetShapeAtIndex(0)->GetFrame();
	double xMin = firstFrame.left;
	double xMax = firstFrame.left + firstFrame.width;

	for (int i = 0; i < m_shapes.GetShapesCount(); i++)
	{
		auto shape = m_shapes.GetShapeAtIndex(i);
		RectangleDouble frame = shape->GetFrame();
		xMin = std::min(xMin, frame.left);
		xMax = std::max(xMax, frame.left + frame.width);
	}

	return xMax - xMin;
}

double CSlide::GetHeight() const
{
	if (m_shapes.GetShapesCount() == 0)
	{
		return 0.0;
	}

	RectangleDouble firstFrame = m_shapes.GetShapeAtIndex(0)->GetFrame();
	double yMin = firstFrame.top;
	double yMax = firstFrame.top + firstFrame.height;

	for (int i = 0; i < m_shapes.GetShapesCount(); i++)
	{
		auto shape = m_shapes.GetShapeAtIndex(i);
		RectangleDouble frame = shape->GetFrame();
		yMin = std::min(yMin, frame.top);
		yMax = std::max(yMax, frame.top + frame.height);
	}

	return yMax - yMin;
}

void CSlide::Draw(ICanvas & canvas)
{
	for (int i = 0; i < m_shapes.GetShapesCount(); i++)
	{
		auto shape = m_shapes.GetShapeAtIndex(i);
		shape->Draw(canvas);
	}
}

size_t CSlide::GetShapesCount()const
{
	return m_shapes.GetShapesCount();
}

void CSlide::InsertShape(const std::shared_ptr<IShape> & shape)
{
	m_shapes.InsertShape(shape);
}

std::shared_ptr<IShape> CSlide::GetShapeAtIndex(size_t index) const
{
	return m_shapes.GetShapeAtIndex(index);
}

void CSlide::RemoveShapeAtIndex(size_t index)
{
	m_shapes.RemoveShapeAtIndex(index);
}