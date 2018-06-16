#pragma once
#include "Command.h"
#include "stdafx.h"

template <typename T>
class CInsertItemCommand : public CCommand
{
public:
	CInsertItemCommand(T& container, typename T::value_type const& item, boost::optional<size_t> const& position = boost::none);

private:
	void DoExecute()override;
	void DoUnexecute()override;

	T& m_container;
	typename T::value_type m_item;
	boost::optional<size_t> m_position;
};

template <typename T>
CInsertItemCommand<T>::CInsertItemCommand(T& container, typename T::value_type const& item, boost::optional<size_t> const& position)
	: m_container(container), m_item(item), m_position(position)
{
}

template <typename T>
void CInsertItemCommand<T>::DoExecute()
{
	const size_t containerSize = m_container.size();
	if (m_position == boost::none || containerSize == (*m_position))
	{
		m_container.push_back(m_item);
	}
	else if (containerSize > (*m_position))
	{
		auto it = m_container.begin();
		std::advance(it, (*m_position));
		m_container.insert(it, m_item);
	}
	else
	{
		throw std::runtime_error("Index out of range!");
	}
}

template <typename T>
void CInsertItemCommand<T>::DoUnexecute()
{
	const size_t containerSize = m_container.size();
	if (containerSize == 0)
	{
		throw std::runtime_error("Index out of range!");
	}

	const size_t lastItemIndex = containerSize - 1;
	if (m_position == boost::none || (*m_position) == lastItemIndex)
	{
		m_container.pop_back();
	}
	else if ((*m_position) < lastItemIndex)
	{
		auto it = m_container.begin();
		std::advance(it, (*m_position));
		m_container.erase(it);
	}
	else
	{
		throw std::runtime_error("Index out of range!");
	}
}