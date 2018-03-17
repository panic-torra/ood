#pragma once
#include "stdafx.h"
#include "Color.h"

static Color GetColorFromString(std::string stringColor)
{
	Color result = Color::black;
	for (auto &i : COLOR_TO_STRING_MAP)
	{
		if (i.second == stringColor)
		{
			result = i.first;
			break;
		}
	}

	return result;
}

static std::string GetColorStringValue(Color color)
{
	return COLOR_TO_STRING_MAP.at(color);
}