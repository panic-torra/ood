#pragma once
#include "IGroupShape.h"
#include "Shapes.h"
#include "CompositeStyle.h"

class CGroupShape : public IGroupShape
{
public:
	CGroupShape();

	virtual void Draw(ICanvas & canvas) override;

	virtual std::shared_ptr<IStyle> GetOutlineStyle() const override;
	virtual std::shared_ptr<IStyle> GetFillStyle() const override;

	virtual std::shared_ptr<IGroupShape> GetGroup() override;
	virtual std::shared_ptr<const IGroupShape> GetGroup() const override;

	virtual RectangleDouble GetFrame()const override;
	virtual void SetFrame(const RectangleDouble & rect) override;

	virtual size_t GetShapesCount()const override;
	virtual void InsertShape(const std::shared_ptr<IShape> & shape) override;
	virtual void RemoveShapeAtIndex(size_t index) override;
	virtual std::shared_ptr<IShape> GetShapeAtIndex(size_t index)const override;

private:
	std::shared_ptr<CShapes> m_shapes;
	std::shared_ptr<CCompositeStyle> m_fillStyle;
	std::shared_ptr<CCompositeStyle> m_outlineStyle;
};