#pragma once
#include "stdafx.h"

enum Color {
	black,
	blue,
	green,
	pink,
	red,
	yellow
};

const std::map<Color, std::string> COLOR_TO_STRING_MAP = {
	{ Color::black, "black" },
	{ Color::blue, "blue" },
	{ Color::green, "green" },
	{ Color::pink, "pink" },
	{ Color::red, "red" },
	{ Color::yellow, "yellow" }
};