#include "stdafx.h"
#include "../DocumentEditor/ChangeItemCommand.h"

struct ItemsFixture
{
	int item1;
	int item2;

	ItemsFixture()
		: item1(1), item2(2)
	{
	}
};

BOOST_FIXTURE_TEST_SUITE(ChangeItemCommand, ItemsFixture)
	BOOST_AUTO_TEST_CASE(can_be_executed)
	{
		CChangeItemCommand<int> command(item1, item2);
		BOOST_CHECK_NO_THROW(command.Execute());
		BOOST_CHECK_EQUAL(item1, 2);
	}

	BOOST_AUTO_TEST_CASE(can_be_unexecuted)
	{
		CChangeItemCommand<int> command(item1, item2);
		BOOST_CHECK_NO_THROW(command.Execute());
		BOOST_CHECK_NO_THROW(command.Unexecute());
		BOOST_CHECK_EQUAL(item1, 1);
	}

	BOOST_AUTO_TEST_CASE(cant_be_executed_twice)
	{
		CChangeItemCommand<int> command(item1, item2);
		BOOST_CHECK_NO_THROW(command.Execute());
		BOOST_CHECK_THROW(command.Execute(), std::runtime_error);
		BOOST_CHECK_EQUAL(item1, 2);
	}

	BOOST_AUTO_TEST_CASE(cant_be_unexecuted_when_no_executed)
	{
		CChangeItemCommand<int> command(item1, item2);
		BOOST_CHECK_THROW(command.Unexecute(), std::runtime_error);
		BOOST_CHECK_EQUAL(item1, 1);
	}

	BOOST_AUTO_TEST_CASE(cant_be_unexecuted_twice)
	{
		CChangeItemCommand<int> command(item1, item2);
		BOOST_CHECK_NO_THROW(command.Execute());
		BOOST_CHECK_NO_THROW(command.Unexecute());
		BOOST_CHECK_THROW(command.Unexecute(), std::runtime_error);
		BOOST_CHECK_EQUAL(item1, 1);
	}
BOOST_AUTO_TEST_SUITE_END()