#pragma once
#include "IShape.h"

class CShape : public IShape
{
public:
	virtual void SetColor(Color color) override;
	virtual Color GetColor() const override;

private:
	Color m_color = Color::black;
};

