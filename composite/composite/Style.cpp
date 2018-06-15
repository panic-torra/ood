#include "stdafx.h"
#include "Style.h"

using namespace std;

CStyle::CStyle(bool isEnabled, RGBAColor color)
	: m_isEnabled(isEnabled)
	, m_color(color)
{
}

optional<bool> CStyle::IsEnabled()const
{
	return m_isEnabled;
}

void CStyle::Enable(bool enable)
{
	m_isEnabled = enable;
}

optional<RGBAColor> CStyle::GetColor()const
{
	return m_color;
}

void CStyle::SetColor(RGBAColor color)
{
	m_color = color;
	m_isEnabled = true;
}