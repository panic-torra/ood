#pragma once
#include "stdafx.h"
#include "IShapeFactory.h"
#include "Color.h"
#include "Shape.h"

class CShapeFactory : public IShapeFactory
{
public:
	std::unique_ptr<CShape> CreateShape(std::string const& description) override;

private:
	static std::unique_ptr<CShape> GetEllipse(const std::vector<std::string> & params);
	static std::unique_ptr<CShape> GetPolygon(const std::vector<std::string> & params);
	static std::unique_ptr<CShape> GetRectangle(const std::vector<std::string> & params);
	static std::unique_ptr<CShape> GetTriangle(const std::vector<std::string> & params);
};
