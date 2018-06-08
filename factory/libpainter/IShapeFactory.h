#pragma once
#include "stdafx.h"
#include "Shape.h"

class IShapeFactory
{
public:
	virtual std::unique_ptr<CShape> CreateShape(const std::string & description) = 0;
	virtual ~IShapeFactory() = default;
};