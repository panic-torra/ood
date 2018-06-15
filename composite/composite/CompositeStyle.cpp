#include "stdafx.h"
#include "CompositeStyle.h"
#include "Style.h"

using namespace std;

void CCompositeStyle::Enable(bool enable)
{
	if (m_shapes->GetShapesCount() != 0)
	{
		for (int i = 0; i < m_shapes->GetShapesCount(); i++)
		{
			auto shape = m_shapes->GetShapeAtIndex(i);
			auto style = m_getStyle->GetStyle(shape);
			style->Enable(true);
		}
	}
}

void CCompositeStyle::SetColor(RGBAColor color)
{
	if (m_shapes->GetShapesCount() != 0)
	{
		for (int i = 0; i < m_shapes->GetShapesCount(); i++)
		{
			auto shape = m_shapes->GetShapeAtIndex(i);
			auto style = m_getStyle->GetStyle(shape);
			style->SetColor(color);
		}
	}
}

optional<bool> CCompositeStyle::IsEnabled()const
{
	optional<bool> isEnabled;

	if (m_shapes->GetShapesCount() == 1)
	{
		auto shape = m_shapes->GetShapeAtIndex(0);
		auto style = m_getStyle->GetStyle(shape);
		return style->IsEnabled();
	}

	if (m_shapes->GetShapesCount() > 1)
	{
		auto firstShape = m_shapes->GetShapeAtIndex(0);
		auto firstStyle = m_getStyle->GetStyle(firstShape);

		isEnabled = firstStyle->IsEnabled();
		for (int i = 1; i < m_shapes->GetShapesCount(); i++)
		{
			auto shape = m_shapes->GetShapeAtIndex(i);
			auto style = m_getStyle->GetStyle(shape);
			if (style->IsEnabled() != isEnabled)
			{
				isEnabled = boost::none;
				break;
			}
		}
	}

	return isEnabled;
}

optional<RGBAColor> CCompositeStyle::GetColor()const
{
	optional<RGBAColor> resultColor;

	if (m_shapes->GetShapesCount() == 1)
	{
		auto style = m_getStyle->GetStyle(m_shapes->GetShapeAtIndex(0));
		if (style->IsEnabled())
		{
			return style->GetColor();
		}
	}

	if (m_shapes->GetShapesCount() > 1)
	{
		auto firstShape = m_shapes->GetShapeAtIndex(0);
		auto firstStyle = m_getStyle->GetStyle(firstShape);

		if (firstStyle->IsEnabled())
		{
			resultColor = firstStyle->GetColor();
			for (int i = 1; i < m_shapes->GetShapesCount(); i++)
			{
				auto shape = m_shapes->GetShapeAtIndex(i);
				auto style = m_getStyle->GetStyle(shape);

				if (style->GetColor() != resultColor || !style->IsEnabled())
				{					
					resultColor = boost::none;
					break;
				}
			}
		}
	}

	return resultColor;
}