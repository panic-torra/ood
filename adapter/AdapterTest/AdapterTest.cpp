#include "stdafx.h"
#include "../AdapterClass/AdapterClass.h"

BOOST_AUTO_TEST_SUITE(adapter)
	BOOST_AUTO_TEST_CASE(can_move_and_draw_line)
	{
		stringstream stream;

		app::CModernGraphicsRendererAdapter adapter(stream);

		adapter.MoveTo(250, 250);
		adapter.LineTo(260, 260);

		auto expectedResult = R"(<draw>
(<line fromX=250 fromY=250 toX=260 toY=260/>)
)";

		BOOST_CHECK_EQUAL(stream.str(), expectedResult);
	}
BOOST_AUTO_TEST_SUITE_END()