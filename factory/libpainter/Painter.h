#pragma once
#include "IPainter.h"

class CPainter : public IPainter 
{
public:
	void DrawPicture(CPictureDraft & draft, ICanvas & canvas) override;

	CPainter();
	~CPainter();
};