#pragma once
#include "stdafx.h"
#include "Shape.h"

struct IShapeFactory
{
	virtual std::unique_ptr<CShape> CreateShape(const std::string & description) = 0;
};