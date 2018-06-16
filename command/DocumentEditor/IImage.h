#pragma once
#include "stdafx.h"

struct ImageSize
{
	ImageSize(unsigned width, unsigned height)
		: width(width), height(height)
	{}
	unsigned width, height;
};

class IImage
{
public:
	virtual boost::filesystem::path GetPath()const = 0;

	virtual unsigned GetWidth()const = 0;
	virtual unsigned GetHeight()const = 0;

	virtual void Resize(unsigned width, unsigned height) = 0;

	virtual ~IImage() = default;
};
