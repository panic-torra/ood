#include "stdafx.h"
#include "../AdapterClass/AdapterClass.h"

BOOST_AUTO_TEST_SUITE(adapter)
	BOOST_AUTO_TEST_CASE(can_move_and_draw_line)
	{
		boost::test_tools::output_test_stream stream;

		modern_graphics_lib::CModernGraphicsRenderer renderer(stream);
		app::CModernGraphicsRendererAdapter adapter(renderer);

		adapter.MoveTo(250, 250);
		adapter.LineTo(260, 260);

		BOOST_CHECK(stream.is_equal(
			"<draw>\n"
			"<line fromX=\"250\" fromY=\"250\" toX=\"260\" toY=\"260\"/>\n"
		));
	}
BOOST_AUTO_TEST_SUITE_END()