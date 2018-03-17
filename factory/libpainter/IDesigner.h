#pragma once
#include <istream>

class CPictureDraft;

class IDesigner
{
public:
	virtual CPictureDraft CreateDraft(std::istream &data) = 0;
};
