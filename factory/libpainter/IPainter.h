#pragma once
#include "PictureDraft.h"
#include "ICanvas.h"

class IPainter
{
public:
	virtual void DrawPicture(CPictureDraft & draft, ICanvas & canvas) = 0;
	virtual ~IPainter() = default;
};