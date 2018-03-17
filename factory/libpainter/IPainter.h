#pragma once
#include "PictureDraft.h"
#include "ICanvas.h"

class IPainter
{
	virtual void DrawPicture(CPictureDraft & draft, ICanvas & canvas) = 0;
};