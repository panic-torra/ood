#include "stdafx.h"
#include "../AdapterPro/AdapterPro.h"

BOOST_AUTO_TEST_SUITE(AdapterPro)
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
			"  <color r=\"0\" g=\"0\" b=\"0\" a=\"0\" />\n"
			"</line>\n"
		));
	}

	BOOST_AUTO_TEST_CASE(can_draw_triangle)
	{
		boost::test_tools::output_test_stream stream;

		modern_graphics_lib::CModernGraphicsRenderer renderer(stream);
		app::CModernGraphicsRendererAdapter adapter(renderer);
		shape_drawing_lib::CCanvasPainter painter(adapter);

		shape_drawing_lib::CTriangle triangle({ 10, 15 }, { 100, 200 }, { 150, 250 }, 0xFF0000);
		painter.Draw(triangle);

		BOOST_CHECK(stream.is_equal(
			"<draw>\n"
			"<line fromX=\"10\" fromY=\"15\" toX=\"100\" toY=\"200\"/>\n"
			"  <color r=\"1\" g=\"0\" b=\"0\" a=\"1\" />\n"
			"</line>\n"
			"<line fromX=\"100\" fromY=\"200\" toX=\"150\" toY=\"250\"/>\n"
			"  <color r=\"1\" g=\"0\" b=\"0\" a=\"1\" />\n"
			"</line>\n"
			"<line fromX=\"150\" fromY=\"250\" toX=\"10\" toY=\"15\"/>\n"
			"  <color r=\"1\" g=\"0\" b=\"0\" a=\"1\" />\n"
			"</line>\n"
		));
	}

	BOOST_AUTO_TEST_CASE(can_draw_rectangle)
	{
		boost::test_tools::output_test_stream stream;

		modern_graphics_lib::CModernGraphicsRenderer renderer(stream);
		app::CModernGraphicsRendererAdapter adapter(renderer);
		shape_drawing_lib::CCanvasPainter painter(adapter);

		shape_drawing_lib::CRectangle rectangle({ 30, 40 }, 18, 24, 0xFF0000);
		painter.Draw(rectangle);

		BOOST_CHECK(stream.is_equal(
			"<draw>\n"
			"<line fromX=\"30\" fromY=\"40\" toX=\"48\" toY=\"40\"/>\n"
			"  <color r=\"1\" g=\"0\" b=\"0\" a=\"1\" />\n"
			"</line>\n"
			"<line fromX=\"48\" fromY=\"40\" toX=\"48\" toY=\"64\"/>\n"
			"  <color r=\"1\" g=\"0\" b=\"0\" a=\"1\" />\n"
			"</line>\n"
			"<line fromX=\"48\" fromY=\"64\" toX=\"30\" toY=\"64\"/>\n"
			"  <color r=\"1\" g=\"0\" b=\"0\" a=\"1\" />\n"
			"</line>\n"
			"<line fromX=\"30\" fromY=\"64\" toX=\"30\" toY=\"40\"/>\n"
			"  <color r=\"1\" g=\"0\" b=\"0\" a=\"1\" />\n"
			"</line>\n"
		));
	}
BOOST_AUTO_TEST_SUITE_END()