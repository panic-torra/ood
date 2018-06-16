#include "stdafx.h"
#include "../DocumentEditor/DeleteItemCommand.h"

struct EmptyContainerFixture
{
	std::list<int> list;
};

struct NonEmptyContainerFixture : public EmptyContainerFixture
{
	NonEmptyContainerFixture()
	{
		list.push_back(0);
		list.push_back(1);
		list.push_back(2);
	}

	void CheckContainerIsNotChanged()
	{
		auto it = list.begin();
		BOOST_CHECK_EQUAL(list.size(), 3);
		BOOST_CHECK_EQUAL((*it), 0);
		BOOST_CHECK_EQUAL((*++it), 1);
		BOOST_CHECK_EQUAL((*++it), 2);
	}
};

BOOST_FIXTURE_TEST_SUITE(DeleteItemCommand, NonEmptyContainerFixture)
	BOOST_AUTO_TEST_SUITE(while_executing)
		BOOST_AUTO_TEST_CASE(removes_from_begin)
		{
			CDeleteItemCommand<std::list<int>> command(list, 0);
			BOOST_CHECK_NO_THROW(command.Execute());

			auto it = list.begin();
			BOOST_CHECK_EQUAL(list.size(), 2);
			BOOST_CHECK_EQUAL((*it), 1);
			BOOST_CHECK_EQUAL((*++it), 2);
		}

		BOOST_AUTO_TEST_CASE(removes_from_center)
		{
			CDeleteItemCommand<std::list<int>> command(list, 1);
			BOOST_CHECK_NO_THROW(command.Execute());

			auto it = list.begin();
			BOOST_CHECK_EQUAL(list.size(), 2);
			BOOST_CHECK_EQUAL((*it), 0);
			BOOST_CHECK_EQUAL((*++it), 2);
		}

		BOOST_AUTO_TEST_CASE(removes_from_end)
		{
			CDeleteItemCommand<std::list<int>> command(list, 2);
			BOOST_CHECK_NO_THROW(command.Execute());

			auto it = list.begin();
			BOOST_CHECK_EQUAL(list.size(), 2);
			BOOST_CHECK_EQUAL((*it), 0);
			BOOST_CHECK_EQUAL((*++it), 1);
		}

		BOOST_FIXTURE_TEST_CASE(returns_error_when_deleted_from_empty_container, EmptyContainerFixture)
		{
			CDeleteItemCommand<std::list<int>> command(list, 0);
			BOOST_CHECK_THROW(command.Execute(), std::runtime_error);
			BOOST_CHECK_EQUAL(list.size(), 0);
		}

		BOOST_AUTO_TEST_CASE(returns_error_when_deleted_from_container_out_of_range)
		{
			CDeleteItemCommand<std::list<int>> command(list, 3);
			BOOST_CHECK_THROW(command.Execute(), std::runtime_error);
			CheckContainerIsNotChanged();
		}

		BOOST_AUTO_TEST_CASE(returns_error_when_executed_twice)
		{
			CDeleteItemCommand<std::list<int>> command(list, 2);
			BOOST_CHECK_NO_THROW(command.Execute());
			BOOST_CHECK_THROW(command.Execute(), std::runtime_error);

			auto it = list.begin();
			BOOST_CHECK_EQUAL(list.size(), 2);
			BOOST_CHECK_EQUAL((*it), 0);
			BOOST_CHECK_EQUAL((*++it), 1);
		}

	BOOST_AUTO_TEST_SUITE_END()

	BOOST_AUTO_TEST_SUITE(while_unexecuting)
		BOOST_FIXTURE_TEST_CASE(inserts_in_empty_container, EmptyContainerFixture)
		{
			list.push_back(0);
			CDeleteItemCommand<std::list<int>> command(list, 0);
			BOOST_CHECK_NO_THROW(command.Execute());
			BOOST_CHECK_NO_THROW(command.Unexecute());

			auto it = list.begin();
			BOOST_CHECK_EQUAL(list.size(), 1);
			BOOST_CHECK_EQUAL((*it), 0);
		}

		BOOST_AUTO_TEST_CASE(inserts_in_begin)
		{
			CDeleteItemCommand<std::list<int>> command(list, 0);
			BOOST_CHECK_NO_THROW(command.Execute());
			BOOST_CHECK_NO_THROW(command.Unexecute());
			CheckContainerIsNotChanged();
		}

		BOOST_AUTO_TEST_CASE(inserts_in_center)
		{
			CDeleteItemCommand<std::list<int>> command(list, 1);
			BOOST_CHECK_NO_THROW(command.Execute());
			BOOST_CHECK_NO_THROW(command.Unexecute());
			CheckContainerIsNotChanged();
		}

		BOOST_AUTO_TEST_CASE(inserts_at_back)
		{
			CDeleteItemCommand<std::list<int>> command(list, 2);
			BOOST_CHECK_NO_THROW(command.Execute());
			BOOST_CHECK_NO_THROW(command.Unexecute());
			CheckContainerIsNotChanged();
		}

		BOOST_AUTO_TEST_CASE(returns_error_when_trying_to_insert_out_of_range)
		{
			CDeleteItemCommand<std::list<int>> command(list, 2);
			BOOST_CHECK_NO_THROW(command.Execute());
			list.pop_back();

			BOOST_CHECK_THROW(command.Unexecute(), std::runtime_error);

			auto it = list.begin();
			BOOST_CHECK_EQUAL(list.size(), 1);
			BOOST_CHECK_EQUAL((*it), 0);
		}

		BOOST_AUTO_TEST_CASE(returns_error_when_no_executed)
		{
			CDeleteItemCommand<std::list<int>> command(list, 2);
			BOOST_CHECK_THROW(command.Unexecute(), std::runtime_error);
			CheckContainerIsNotChanged();
		}

		BOOST_AUTO_TEST_CASE(returns_error_when_unexecuted_twice)
		{
			CDeleteItemCommand<std::list<int>> command(list, 2);
			BOOST_CHECK_NO_THROW(command.Execute());
			BOOST_CHECK_NO_THROW(command.Unexecute());
			BOOST_CHECK_THROW(command.Unexecute(), std::runtime_error);

			CheckContainerIsNotChanged();
		}

	BOOST_AUTO_TEST_SUITE_END()

	BOOST_AUTO_TEST_CASE(can_be_executed_multiple_times)
	{
		CDeleteItemCommand<std::list<int>> command(list, 2);
		BOOST_CHECK_NO_THROW(command.Execute());
		BOOST_CHECK_NO_THROW(command.Unexecute());
		BOOST_CHECK_NO_THROW(command.Execute());
		BOOST_CHECK_NO_THROW(command.Unexecute());
	}

BOOST_AUTO_TEST_SUITE_END()