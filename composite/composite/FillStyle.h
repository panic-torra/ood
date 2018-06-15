#pragma once
#include "IStyleBehavior.h"

class CFillStyle : public IStyleBehavior
{
public:
	CFillStyle();

	std::shared_ptr<IStyle> GetStyle(std::shared_ptr<IShape> shape) override;

	~CFillStyle();
};

