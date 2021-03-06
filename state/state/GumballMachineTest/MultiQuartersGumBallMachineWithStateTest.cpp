#include "stdafx.h"
#include "../GumballMachine/MultiQuartersGumBallMachineWithState.h"

using namespace std;

struct Multi_gumball_machine_with_state_
{
	boost::test_tools::output_test_stream outputStream;
	with_state_multi::CGumballMachine machine{ 3 };
};

BOOST_FIXTURE_TEST_SUITE(Multi_gumball_machine_with_state, Multi_gumball_machine_with_state_)
	BOOST_AUTO_TEST_SUITE(when_created)
		BOOST_AUTO_TEST_CASE(has_valid_init_state)
		{
			string expectedOutput = R"(
Mighty Gumball, Inc.
C++-enabled Standing Gumball Model #2016 (with state)
Inventory: 3 gumballs
Quarters: 0
Machine is waiting for quarter
)";

			outputStream << machine.ToString();
			BOOST_CHECK(outputStream.is_equal(expectedOutput));
		}

		BOOST_AUTO_TEST_CASE(increase_quarters_count_after_adding)
		{
			string expectedOutput = R"(
Mighty Gumball, Inc.
C++-enabled Standing Gumball Model #2016 (with state)
Inventory: 3 gumballs
Quarters: 3
Machine is waiting for turn of crank
)";
			machine.InsertQuarter();
			machine.InsertQuarter();
			machine.InsertQuarter();

			outputStream << machine.ToString();
			BOOST_CHECK(outputStream.is_equal(expectedOutput));

			expectedOutput = R"(
Mighty Gumball, Inc.
C++-enabled Standing Gumball Model #2016 (with state)
Inventory: 3 gumballs
Quarters: 5
Machine is waiting for turn of crank
)";
			machine.InsertQuarter();
			machine.InsertQuarter();

			outputStream.str("");
			outputStream << machine.ToString();
			BOOST_CHECK(outputStream.is_equal(expectedOutput));
		}

		BOOST_AUTO_TEST_CASE(return_quarters)
		{
			string expectedOutput = R"(
Mighty Gumball, Inc.
C++-enabled Standing Gumball Model #2016 (with state)
Inventory: 3 gumballs
Quarters: 0
Machine is waiting for quarter
)";
			machine.InsertQuarter();
			machine.InsertQuarter();
			machine.InsertQuarter();
			machine.InsertQuarter();
			machine.InsertQuarter();
			machine.EjectQuarter();

			outputStream << machine.ToString();
			BOOST_CHECK(outputStream.is_equal(expectedOutput));
		}

		BOOST_AUTO_TEST_CASE(return_quarters_in_solid_state)
		{
			string expectedOutput = R"(
Mighty Gumball, Inc.
C++-enabled Standing Gumball Model #2016 (with state)
Inventory: 0 gumballs
Quarters: 0
Machine is sold out
)";
			machine.InsertQuarter();
			machine.TurnCrank();

			machine.InsertQuarter();
			machine.TurnCrank();

			machine.InsertQuarter();
			machine.TurnCrank();

			machine.InsertQuarter();
			machine.InsertQuarter();
			machine.EjectQuarter();

			outputStream << machine.ToString();
			BOOST_CHECK(outputStream.is_equal(expectedOutput));
		}

		BOOST_AUTO_TEST_CASE(leave_machine_to_has_quarters_state_after_dispense)
		{
			string expectedOutput = R"(
Mighty Gumball, Inc.
C++-enabled Standing Gumball Model #2016 (with state)
Inventory: 2 gumballs
Quarters: 3
Machine is waiting for turn of crank
)";
			machine.InsertQuarter();
			machine.InsertQuarter();
			machine.InsertQuarter();
			machine.InsertQuarter();
			machine.TurnCrank();

			outputStream << machine.ToString();
			BOOST_CHECK(outputStream.is_equal(expectedOutput));
		}
	BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()