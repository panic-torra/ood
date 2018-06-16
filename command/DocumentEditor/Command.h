#pragma once
#include "ICommand.h"

class CCommand : public ICommand
{
public:
	void Execute()override final;
	void Unexecute()override final;

private:
	virtual void DoExecute() = 0;
	virtual void DoUnexecute() = 0;

	bool m_isExecuted = false;
};