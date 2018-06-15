#pragma once
#include "IStyle.h"
#include "IShape.h"

class IStyleBehavior
{
public:
	virtual std::shared_ptr<IStyle> GetStyle(std::shared_ptr<IShape> shape) = 0;
};

