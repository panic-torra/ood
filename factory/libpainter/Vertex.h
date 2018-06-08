#pragma once

struct Vertex
{
	Vertex(float inputX = 0, float inputY = 0)
		: x(inputX)
		, y(inputY)
	{};

	float x;
	float y;
};