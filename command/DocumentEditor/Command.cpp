#include "stdafx.h"
#include "Command.h"

void CCommand::Execute()
{
	if (m_isExecuted == true)
	{
		throw std::runtime_error("The command is already executed!");
	}
	DoExecute();
	m_isExecuted = true;
}

void CCommand::Unexecute()
{
	if (m_isExecuted == false)
	{
		throw std::runtime_error("The command is not executed!");
	}
	DoUnexecute();
	m_isExecuted = false;
}