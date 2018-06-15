#pragma once
#include "IStyleBehavior.h"

class COutlineStyle : public IStyleBehavior
{
public:
	COutlineStyle();

	std::shared_ptr<IStyle> GetStyle(std::shared_ptr<IShape> shape) override;

	~COutlineStyle();
};

