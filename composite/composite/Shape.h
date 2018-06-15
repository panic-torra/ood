#pragma once
#include "ICanvas.h"
#include "IShape.h"

class CShape : public IShape
{
public:
	CShape();
	
	virtual std::shared_ptr<IStyle> GetOutlineStyle()const override;
	virtual std::shared_ptr<IStyle> GetFillStyle()const override;

	virtual std::shared_ptr<IGroupShape> GetGroup() override;
	virtual std::shared_ptr<const IGroupShape> GetGroup() const override;

	void SetCanvasStyles(ICanvas & canvas);
protected:
	std::shared_ptr<IStyle> m_outlineStyle;
	std::shared_ptr<IStyle> m_fillStyle;
};