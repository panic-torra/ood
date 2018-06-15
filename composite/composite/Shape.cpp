#include "stdafx.h"
#include "Shape.h"
#include "Style.h"

using namespace std;

CShape::CShape()
{
	m_fillStyle = make_shared<CStyle>(false, NULL);
	m_outlineStyle = make_shared<CStyle>(true, 0);
}

std::shared_ptr<IStyle> CShape::GetOutlineStyle()const
{
	return m_outlineStyle;
}

std::shared_ptr<IStyle> CShape::GetFillStyle()const
{
	return m_fillStyle;
}

std::shared_ptr<IGroupShape> CShape::GetGroup()
{
	return nullptr;
}

std::shared_ptr<const IGroupShape> CShape::GetGroup() const
{
	return nullptr;
}

void CShape::SetCanvasStyles(ICanvas & canvas)
{
	if (m_outlineStyle->GetColor()) 
	{
		canvas.SetLineColor(*m_outlineStyle->GetColor());
	}
	
	if (m_fillStyle->IsEnabled())
	{
		canvas.BeginFill(*m_fillStyle->GetColor());
	}
}