#pragma once
#include "stdafx.h"
#include "Triangle.h"
#include "Rectangle.h"
#include "Ellipse.h"
#include "RegularPolygon.h"
#include "ShapeFactory.h"
#include "StringUtils.h"
#include "ColorUtils.h"

std::unique_ptr<CShape> CShapeFactory::CreateShape(const std::string & description)
{
	std::vector<std::string> params = GetInputParamsHelper::GetParams(description);
	std::string shape = boost::algorithm::to_lower_copy(params.at(0));

	if (shape == "triangle")
	{
		return GetTriangle(params);
	}
	else if (shape == "rectangle")
	{
		return GetRectangle(params);
	}
	else if (shape == "ellipse")
	{
		return GetEllipse(params);
	}
	else if (shape == "polygon")
	{
		return GetPolygon(params);
	}

	throw std::invalid_argument("Unknown shape");
}

std::unique_ptr<CShape> CShapeFactory::GetTriangle(const std::vector<std::string> & params)
{
	Color color = GetColorParamsHelper::GetColorFromString(params.at(1));
	Vertex v1 = { stof(params.at(2)), stof(params.at(3)) };
	Vertex v2 = { stof(params.at(4)), stof(params.at(5)) };
	Vertex v3 = { stof(params.at(6)), stof(params.at(7)) };

	return std::make_unique<CTriangle>(v1, v2, v3, color);
}

std::unique_ptr<CShape> CShapeFactory::GetRectangle(const std::vector<std::string> & params)
{
	Color color = GetColorParamsHelper::GetColorFromString(params.at(1));
	Vertex leftTop = { stof(params.at(2)), stof(params.at(3)) };
	Vertex rightTop = { stof(params.at(4)), stof(params.at(5)) };

	return std::make_unique<CRectangle>(leftTop, rightTop, color);
}

std::unique_ptr<CShape> CShapeFactory::GetEllipse(const std::vector<std::string> & params)
{
	Color color = GetColorParamsHelper::GetColorFromString(params.at(1));
	Vertex center = { stof(params.at(2)), stof(params.at(3)) };
	float width = stof(params.at(4));
	float height = stof(params.at(5));

	return std::make_unique<CEllipse>(center, width, height, color);
}

std::unique_ptr<CShape> CShapeFactory::GetPolygon(const std::vector<std::string> & params)
{
	Color color = GetColorParamsHelper::GetColorFromString(params.at(1));
	Vertex center = { stof(params.at(2)), stof(params.at(3)) };
	float radius = stof(params.at(4));
	int vertexCount = stoi(params.at(5));

	return std::make_unique<CRegularPolygon>(center, radius, vertexCount, color);
}