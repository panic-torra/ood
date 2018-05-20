#pragma once
#include "IShape.h"

class CShape : public IShape
{
public:
	void SetColor(Color color) override;
	Color GetColor() const override;

	void Draw(ICanvas &canvas) const override;
private:
	Color m_color = Color::black;
};

