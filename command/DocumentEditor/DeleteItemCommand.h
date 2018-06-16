#pragma once
#include "Command.h"
#include <boost\optional.hpp>

template <typename T>
class CDeleteItemCommand : public CCommand
{
public:
	CDeleteItemCommand(T& container, boost::optional<size_t> const& position);

private:
	void DoExecute()override;
	void DoUnexecute()override;

	T& m_container;
	typename T::value_type m_item;
	boost::optional<size_t> m_position;
};

template <typename T>
CDeleteItemCommand<T>::CDeleteItemCommand(T& container, boost::optional<size_t> const& position)
	: m_container(container), m_position(position)
{
}

template <typename T>
void CDeleteItemCommand<T>::DoExecute()
{
	size_t containerSize = m_container.size();
	if (containerSize == 0 || (*m_position) > containerSize - 1)
	{
		throw std::runtime_error("Out of range!");
	}

	auto it = m_container.begin();
	std::advance(it, (*m_position));

	m_item = (*it);
	m_container.erase(it);
}

template <typename T>
void CDeleteItemCommand<T>::DoUnexecute()
{
	size_t containerSize = m_container.size();
	if ((containerSize == 0 && (*m_position) != 0) || ((*m_position) > containerSize))
	{
		throw std::runtime_error("Out of range!");
	}

	auto it = m_container.begin();
	std::advance(it, (*m_position));
	m_container.insert(it, m_item);
}