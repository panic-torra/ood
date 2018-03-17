#pragma once
#include "Shape.h"

class CEllipse : public CShape
{
public:
	CEllipse(Vertex const & center, float horizontalRadius, float verticalRadius, Color const & color);

	Vertex GetCenter() const;
	float GetHorizontalRadius() const;
	float GetVerticalRadius() const;

	void Draw(ICanvas &canvas) const override;

private:
	Vertex m_center = { 0, 0 };
	float m_horizontalRadius = 0;
	float m_verticalRadius = 0;
};