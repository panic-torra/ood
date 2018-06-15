#pragma once
#include "IShapes.h"

class CShapes : public IShapes
{
public:
	virtual size_t GetShapesCount()const override;
	virtual void InsertShape(const std::shared_ptr<IShape> & shape) override;	
	virtual void RemoveShapeAtIndex(size_t index) override;
	virtual std::shared_ptr<IShape> GetShapeAtIndex(size_t index)const override;

protected:
	std::vector<std::shared_ptr<IShape>> m_shapes;
};

