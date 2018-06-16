#include "stdafx.h"
#include "../DocumentEditor/InsertItemCommand.h"

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

BOOST_FIXTURE_TEST_SUITE(InsertItemCommand, EmptyContainerFixture)
	BOOST_AUTO_TEST_SUITE(while_executing)
		BOOST_AUTO_TEST_CASE(inserts_in_empty_container_without_position)
		{
			CInsertItemCommand<std::list<int>> command(list, 0);
			BOOST_CHECK_NO_THROW(command.Execute());
			BOOST_CHECK_EQUAL(list.size(), 1);
		}

		BOOST_AUTO_TEST_CASE(inserts_in_empty_container_with_position)
		{
			CInsertItemCommand<std::list<int>> command(list, 0, 0);
			BOOST_CHECK_NO_THROW(command.Execute());
			BOOST_CHECK_EQUAL(list.size(), 1);
		}

		BOOST_FIXTURE_TEST_CASE(inserts_in_non_empty_at_begin, NonEmptyContainerFixture)
		{
			CInsertItemCommand<std::list<int>> command(list, 3, 0);
			BOOST_CHECK_NO_THROW(command.Execute());

			auto it = list.begin();
			BOOST_CHECK_EQUAL(list.size(), 4);
			BOOST_CHECK_EQUAL((*it), 3);
			BOOST_CHECK_EQUAL((*++it), 0);
			BOOST_CHECK_EQUAL((*++it), 1);
			BOOST_CHECK_EQUAL((*++it), 2);
		}

		BOOST_FIXTURE_TEST_CASE(inserts_in_non_empty_at_center, NonEmptyContainerFixture)
		{
			CInsertItemCommand<std::list<int>> command1(list, 3, 1);
			BOOST_CHECK_NO_THROW(command1.Execute());

			CInsertItemCommand<std::list<int>> command2(list, 4, 3);
			BOOST_CHECK_NO_THROW(command2.Execute());

			auto it = list.begin();
			BOOST_CHECK_EQUAL(list.size(), 5);
			BOOST_CHECK_EQUAL((*it), 0);
			BOOST_CHECK_EQUAL((*++it), 3);
			BOOST_CHECK_EQUAL((*++it), 1);
			BOOST_CHECK_EQUAL((*++it), 4);
			BOOST_CHECK_EQUAL((*++it), 2);
		}

		BOOST_FIXTURE_TEST_CASE(inserts_in_non_empty_at_back, NonEmptyContainerFixture)
		{
			CInsertItemCommand<std::list<int>> command1(list, 3);
			BOOST_CHECK_NO_THROW(command1.Execute());

			CInsertItemCommand<std::list<int>> command2(list, 4, 4);
			BOOST_CHECK_NO_THROW(command2.Execute());

			auto it = list.begin();
			BOOST_CHECK_EQUAL(list.size(), 5);
			BOOST_CHECK_EQUAL((*it), 0);
			BOOST_CHECK_EQUAL((*++it), 1);
			BOOST_CHECK_EQUAL((*++it), 2);
			BOOST_CHECK_EQUAL((*++it), 3);
			BOOST_CHECK_EQUAL((*++it), 4);
		}


		BOOST_AUTO_TEST_CASE(returns_error_when_insert_in_empty_container_out_of_range)
		{
			CInsertItemCommand<std::list<int>> command(list, 0, 1);
			BOOST_CHECK_THROW(command.Execute(), std::runtime_error);
			BOOST_CHECK_EQUAL(list.size(), 0);
		}

		BOOST_AUTO_TEST_CASE(returns_error_when_executed_twice)
		{
			CInsertItemCommand<std::list<int>> command(list, 0);
			BOOST_CHECK_NO_THROW(command.Execute());
			BOOST_CHECK_THROW(command.Execute(), std::runtime_error);

			BOOST_CHECK_EQUAL(list.size(), 1);
			auto it1 = list.begin();
			BOOST_CHECK_EQUAL((*it1), 0);
		}
	BOOST_AUTO_TEST_SUITE_END()

	BOOST_AUTO_TEST_SUITE(while_unexecuting)
		BOOST_FIXTURE_TEST_CASE(removes_last_item_from_back, NonEmptyContainerFixture)
		{
			CInsertItemCommand<std::list<int>> command1(list, 3);
			command1.Execute();
			BOOST_CHECK_NO_THROW(command1.Unexecute());

			CheckContainerIsNotChanged();

			CInsertItemCommand<std::list<int>> command2(list, 3, 3);
			command2.Execute();
			BOOST_CHECK_NO_THROW(command2.Unexecute());

			CheckContainerIsNotChanged();
		}

		BOOST_FIXTURE_TEST_CASE(removes_last_item_from_center, NonEmptyContainerFixture)
		{
			CInsertItemCommand<std::list<int>> command1(list, 3, 2);
			command1.Execute();
			BOOST_CHECK_NO_THROW(command1.Unexecute());

			CheckContainerIsNotChanged();

			CInsertItemCommand<std::list<int>> command2(list, 4, 1);
			command2.Execute();
			BOOST_CHECK_NO_THROW(command2.Unexecute());

			CheckContainerIsNotChanged();
		}

		BOOST_FIXTURE_TEST_CASE(removes_last_item_from_start, NonEmptyContainerFixture)
		{
			CInsertItemCommand<std::list<int>> command(list, 3, 0);
			command.Execute();
			BOOST_CHECK_NO_THROW(command.Unexecute());

			CheckContainerIsNotChanged();
		}

		BOOST_AUTO_TEST_CASE(removes_item_from_container_with_one_item)
		{
			CInsertItemCommand<std::list<int>> command1(list, 0, 0);
			command1.Execute();
			BOOST_CHECK_NO_THROW(command1.Unexecute());
			BOOST_CHECK_EQUAL(list.size(), 0);

			CInsertItemCommand<std::list<int>> command2(list, 0);
			command2.Execute();
			BOOST_CHECK_NO_THROW(command2.Unexecute());
			BOOST_CHECK_EQUAL(list.size(), 0);
		}

		BOOST_AUTO_TEST_CASE(returns_error_when_delete_from_empty_container)
		{
			CInsertItemCommand<std::list<int>> command(list, 0);
			command.Execute();

			list.clear();
			BOOST_CHECK_THROW(command.Unexecute(), std::runtime_error);
			BOOST_CHECK_EQUAL(list.size(), 0);
		}

		BOOST_FIXTURE_TEST_CASE(returns_error_when_delete_from_container_out_of_range, NonEmptyContainerFixture)
		{
			CInsertItemCommand<std::list<int>> command(list, 3, 3);
			command.Execute();

			list.pop_back();
			BOOST_CHECK_THROW(command.Unexecute(), std::runtime_error);
			CheckContainerIsNotChanged();
		}

		BOOST_AUTO_TEST_CASE(returns_error_when_not_executed)
		{
			CInsertItemCommand<std::list<int>> command(list, 0);
			BOOST_CHECK_THROW(command.Unexecute(), std::runtime_error);
			BOOST_CHECK_EQUAL(list.size(), 0);
		}

		BOOST_AUTO_TEST_CASE(returns_error_when_unexecuted_twice)
		{
			CInsertItemCommand<std::list<int>> command(list, 0);
			command.Execute();
			BOOST_CHECK_NO_THROW(command.Unexecute());
			BOOST_CHECK_THROW(command.Unexecute(), std::runtime_error);
			BOOST_CHECK_EQUAL(list.size(), 0);
		}
	BOOST_AUTO_TEST_SUITE_END()

	BOOST_AUTO_TEST_CASE(can_be_executed_unexecuted_multiple_times)
	{
		CInsertItemCommand<std::list<int>> command(list, 0);
		BOOST_CHECK_NO_THROW(command.Execute());
		BOOST_CHECK_NO_THROW(command.Unexecute());
		BOOST_CHECK_NO_THROW(command.Execute());
		BOOST_CHECK_NO_THROW(command.Unexecute());
		BOOST_CHECK_EQUAL(list.size(), 0);
	}
BOOST_AUTO_TEST_SUITE_END()