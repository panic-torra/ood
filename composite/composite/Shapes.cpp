#include "stdafx.h"
#include "Shapes.h"

size_t CShapes::GetShapesCount()const
{
	return m_shapes.size();
}

void CShapes::InsertShape(const std::shared_ptr<IShape> & shape)
{
	m_shapes.push_back(shape);
}

std::shared_ptr<IShape> CShapes::GetShapeAtIndex(size_t index) const
{
	return m_shapes.at(index);
}

void CShapes::RemoveShapeAtIndex(size_t index)
{
	if (index >= m_shapes.size())
	{
		throw std::out_of_range("There is no shape with this index.");
	}

	m_shapes.erase(m_shapes.begin() + index);
}