#pragma once
#include "ICanvas.h"

class CSVGCanvas : public ICanvas
{
public:
	CSVGCanvas(std::ostream & ostream);

	virtual void SetLineColor(RGBAColor color) override;
	virtual void MoveTo(double x, double y) override;
	virtual void LineTo(double x, double y) override;
	virtual void DrawEllipse(double left, double top, double width, double height) override;

	virtual void BeginFill(RGBAColor color) override;
	virtual void EndFill() override;

	~CSVGCanvas();
private:
	void OpenPath();
	void ClosePath();
	void PrintLineStyle();
	void PrintFillStyle();

	int m_lineWidth = 1;
	RGBAColor m_lineColor = 0;
	RGBAColor m_fillColor = NULL;
	std::ostream &m_output;
	bool m_isDrawing = false;
};

