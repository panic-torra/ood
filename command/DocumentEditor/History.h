#pragma once
#include "stdafx.h"
#include "IHistory.h"

class CHistory : public IHistory
{
public:
	void Push(std::unique_ptr<ICommand> && command) override;
	void Undo() override;
	void Redo() override;

	bool CanUndo()const override;
	bool CanRedo()const override;

private:
	std::deque<std::unique_ptr<ICommand>> m_commands;
	size_t m_executedCommandsCount = 0;
};
