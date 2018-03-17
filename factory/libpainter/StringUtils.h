#pragma once
#include "stdafx.h"
#include "Color.h"

static std::vector<std::string> GetParams(const std::string & stringParams)
{
	std::stringstream paramsStream(stringParams);
	std::vector<std::string> paramsList;
	std::string param;

	while (paramsStream >> param)
	{
		paramsList.push_back(param);
	}

	return paramsList;
}