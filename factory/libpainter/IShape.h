#pragma once
#include "stdafx.h"
#include "ICanvas.h"
#include "Color.h"

class IShape
{
public:
	virtual void SetColor(Color color) = 0;
	virtual Color GetColor() const = 0;

	virtual void Draw(ICanvas & canvas) const = 0;
	virtual ~IShape() = default;
};