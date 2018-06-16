#include "stdafx.h"
#include "History.h"
#include "Command.h"

void CHistory::Push(std::unique_ptr<ICommand> && command)
{
	if (CanRedo())
	{
		command->Execute();
		m_commands.resize(++m_executedCommandsCount);
		m_commands.back() = std::move(command);
	}
	else
	{
		m_commands.resize(m_commands.size() + 1);
		try
		{
			command->Execute();
			m_commands.back() = std::move(command);
			++m_executedCommandsCount;
		}
		catch (...)
		{
			m_commands.pop_back();
			throw;
		}
	}
}

void CHistory::Undo()
{
	if (!CanUndo())
	{
		throw std::runtime_error("Nothing to undo!");
	}
	m_commands[m_executedCommandsCount - 1]->Unexecute();
	--m_executedCommandsCount;
}

void CHistory::Redo()
{
	if (!CanRedo())
	{
		throw std::runtime_error("Nothing to redo!");
	}
	m_commands[m_executedCommandsCount]->Execute();
	++m_executedCommandsCount;
}

bool CHistory::CanUndo()const
{
	return m_executedCommandsCount != 0;
}

bool CHistory::CanRedo()const
{
	return m_executedCommandsCount != m_commands.size();
}