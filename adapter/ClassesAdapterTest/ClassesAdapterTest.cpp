#include "stdafx.h"
#include "../AdapterClass/AdapterClass.h"

BOOST_AUTO_TEST_SUITE(ClassesAdapter)
	BOOST_AUTO_TEST_CASE(can_move_and_draw_line)
	{
		boost::test_tools::output_test_stream stream;

		app::CModernGraphicsRendererAdapter adapter(stream);

		adapter.MoveTo(0, 10);
		adapter.LineTo(260, 260);
		adapter.LineTo(10, 20);

		BOOST_CHECK(stream.is_equal(
			"<draw>\n"
			"<line fromX=\"0\" fromY=\"10\" toX=\"260\" toY=\"260\"/>\n"
			"<line fromX=\"260\" fromY=\"260\" toX=\"10\" toY=\"20\"/>\n"
		));
	}
BOOST_AUTO_TEST_SUITE_END()