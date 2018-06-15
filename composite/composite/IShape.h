#pragma once
#include "IDrawable.h"
#include "IStyle.h"

class IGroupShape;

class IShape : public IDrawable
{
public:
	virtual RectangleDouble GetFrame()const = 0;
	virtual void SetFrame(const RectangleDouble & rect) = 0;

	virtual std::shared_ptr<IStyle> GetOutlineStyle()const = 0;
	virtual std::shared_ptr<IStyle> GetFillStyle()const = 0;

	virtual std::shared_ptr<IGroupShape> GetGroup() = 0;
	virtual std::shared_ptr<const IGroupShape> GetGroup() const = 0;
	
	virtual ~IShape() = default;
};