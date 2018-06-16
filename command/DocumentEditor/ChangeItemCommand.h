#pragma once
#include "Command.h"

template <typename T>
class CChangeItemCommand : public CCommand
{
public:
	CChangeItemCommand(T& targetItem, T const& newItem);

private:
	void DoExecute()override;
	void DoUnexecute()override;

	T& m_targetItem;
	T m_newItem;
};

template <typename T>
CChangeItemCommand<T>::CChangeItemCommand(T& targetItem, T const& newItem)
	: m_targetItem(targetItem), m_newItem(newItem)
{
}

template <typename T>
void CChangeItemCommand<T>::DoExecute()
{
	std::swap(m_targetItem, m_newItem);
}

template <typename T>
void CChangeItemCommand<T>::DoUnexecute()
{
	std::swap(m_newItem, m_targetItem);
}