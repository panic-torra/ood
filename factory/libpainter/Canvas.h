#pragma once
#include "ICanvas.h"

class CCanvas : public ICanvas
{
public:
	CCanvas(std::ostream & outputStream)
		: m_output(outputStream)
	{
	};

	void SetColor(Color color) override;
	void DrawLine(const Vertex from, const Vertex to) override;
	void DrawEllipse(const Vertex center, const float width, const float height) override;

private:
	Color m_color = Color::black;
	std::string m_canvasContent;
	std::ostream & m_output;
};