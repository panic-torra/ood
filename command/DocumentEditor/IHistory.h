#pragma once
#include "stdafx.h"
#include "ICommand.h"

class IHistory
{
public:
	virtual void Push(std::unique_ptr<ICommand> && command) = 0;
	virtual void Undo() = 0;
	virtual void Redo() = 0;

	virtual bool CanUndo()const = 0;
	virtual bool CanRedo()const = 0;

	virtual ~IHistory() = default;
};