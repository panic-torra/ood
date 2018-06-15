#pragma once
#include "stdafx.h"

template <typename T>
struct Rectangle
{
	T left;
	T top;
	T width;
	T height;
};

typedef Rectangle<double> RectangleDouble;
typedef uint32_t RGBAColor;