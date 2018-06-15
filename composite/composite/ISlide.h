#pragma once
#include "IDrawable.h"
#include "IShapes.h"

class ISlide : public IDrawable, public IShapes
{
public:
	virtual double GetWidth()const = 0;
	virtual double GetHeight()const = 0;

	virtual ~ISlide() = default;
};