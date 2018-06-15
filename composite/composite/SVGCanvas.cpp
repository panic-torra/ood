#include "stdafx.h"
#include "SVGCanvas.h"

using namespace std;

CSVGCanvas::CSVGCanvas(std::ostream & ostream)
	: m_output(ostream)
{
	m_output << "<svg version = \"1.1\" xmlns = \"http://www.w3.org/2000/svg\">" << endl;
}

CSVGCanvas::~CSVGCanvas()
{
	if (m_isDrawing)
	{
		CSVGCanvas::ClosePath();
	}

	m_output << "</svg>";
}


void CSVGCanvas::SetLineColor(RGBAColor color)
{
	m_lineColor = color;
	ClosePath();
}

void CSVGCanvas::BeginFill(RGBAColor color)
{
	m_fillColor = color;
}

void CSVGCanvas::EndFill()
{
	m_fillColor = NULL;
}

void CSVGCanvas::MoveTo(double x, double y)
{
	if (!m_isDrawing)
	{
		OpenPath();
	}

	m_output << "M " << x << "," << y << " ";
}

void CSVGCanvas::LineTo(double x, double y)
{
	if (!m_isDrawing)
	{
		MoveTo(0, 0);
	}

	m_output << "L " << x << "," << y << " ";
}

void CSVGCanvas::DrawEllipse(double left, double top, double width, double height)
{
	if (m_isDrawing)
	{
		ClosePath();
	}

	m_output << " <ellipse";
	PrintLineStyle();
	PrintFillStyle();
	m_output << " cx=\"" << left << "\" cy=\"" << top << "\" rx=\"" << width << "\" ry=\"" << height << "\" />" << endl;
	ClosePath();
}

void CSVGCanvas::OpenPath()
{
	m_isDrawing = true;
	m_output << "<path";
	PrintLineStyle();
	PrintFillStyle();
	m_output << " d=\"";
}

void CSVGCanvas::ClosePath()
{
	if (m_isDrawing)
	{
		m_output << "\" />" << endl;
		m_isDrawing = false;
		EndFill();
	}
}

void CSVGCanvas::PrintLineStyle()
{
	m_output << " stroke=\"#" << std::setfill('0') << std::setw(6) << std::hex << m_lineColor << "\" stroke-width=\"" << m_lineWidth << "\"";
}

void CSVGCanvas::PrintFillStyle()
{
	if (m_fillColor)
	{
		m_output << " fill=\"#" << std::setfill('0') << std::setw(6) << std::hex << m_fillColor << "\"";
	}
	else
	{
		m_output << " fill=\"none\"";
	}
}