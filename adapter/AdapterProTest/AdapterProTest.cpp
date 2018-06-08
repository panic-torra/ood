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

		auto expectedResult = R"(<draw>
<line fromX="250" fromY="250" toX="260" toY="260"/>
  <color r="0" g="0" b="0" a="1" />
</line>
)";

		BOOST_CHECK(stream.is_equal(expectedResult));
	}

	BOOST_AUTO_TEST_CASE(can_draw_triangle)
	{
		boost::test_tools::output_test_stream stream;

		modern_graphics_lib::CModernGraphicsRenderer renderer(stream);
		app::CModernGraphicsRendererAdapter adapter(renderer);
		shape_drawing_lib::CCanvasPainter painter(adapter);

		shape_drawing_lib::CTriangle triangle({ 10, 15 }, { 100, 200 }, { 150, 250 }, 0xFF0000);
		painter.Draw(triangle);

		auto expectedResult = R"(<draw>
<line fromX="10" fromY="15" toX="100" toY="200"/>
  <color r="1" g="0" b="0" a="1" />
</line>
<line fromX="100" fromY="200" toX="150" toY="250"/>
  <color r="1" g="0" b="0" a="1" />
</line>
<line fromX="150" fromY="250" toX="10" toY="15"/>
  <color r="1" g="0" b="0" a="1" />
</line>
)";

		BOOST_CHECK(stream.is_equal(expectedResult));
	}

	BOOST_AUTO_TEST_CASE(can_draw_rectangle)
	{
		boost::test_tools::output_test_stream stream;

		modern_graphics_lib::CModernGraphicsRenderer renderer(stream);
		app::CModernGraphicsRendererAdapter adapter(renderer);
		shape_drawing_lib::CCanvasPainter painter(adapter);

		shape_drawing_lib::CRectangle rectangle({ 30, 40 }, 18, 24, 0xFF0000);
		painter.Draw(rectangle);

		auto expectedResult = R"(<draw>
<line fromX="30" fromY="40" toX="48" toY="40"/>
  <color r="1" g="0" b="0" a="1" />
</line>
<line fromX="48" fromY="40" toX="48" toY="64"/>
  <color r="1" g="0" b="0" a="1" />
</line>
<line fromX="48" fromY="64" toX="30" toY="64"/>
  <color r="1" g="0" b="0" a="1" />
</line>
<line fromX="30" fromY="64" toX="30" toY="40"/>
  <color r="1" g="0" b="0" a="1" />
</line>
)";

		BOOST_CHECK(stream.is_equal(expectedResult));
	}

	BOOST_AUTO_TEST_CASE(handle_exception_when_begin_draw_twice)
	{
		boost::test_tools::output_test_stream stream;

		modern_graphics_lib::CModernGraphicsRenderer renderer(stream);
		renderer.BeginDraw();
		BOOST_CHECK_THROW(renderer.BeginDraw(), logic_error);
	}

	BOOST_AUTO_TEST_CASE(handle_exception_when_begin_draw_without_init)
	{
		boost::test_tools::output_test_stream stream;
		modern_graphics_lib::CModernGraphicsRenderer renderer(stream);
		RGBAColor color;
		BOOST_CHECK_THROW(renderer.DrawLine({ 0, 0 }, { 0, 0 }, color), logic_error);
	}

	BOOST_AUTO_TEST_CASE(handle_exception_when_end_draw_witout_starting)
	{
		boost::test_tools::output_test_stream stream;

		modern_graphics_lib::CModernGraphicsRenderer renderer(stream);
		BOOST_CHECK_THROW(renderer.EndDraw(), logic_error);
	}

	BOOST_AUTO_TEST_CASE(can_end_draw_when_object_removed)
	{
		boost::test_tools::output_test_stream stream;

		modern_graphics_lib::CModernGraphicsRenderer renderer(stream);
		renderer.BeginDraw();
		renderer.~CModernGraphicsRenderer();

		auto expectedResult = R"(<draw>
</draw>
)";

		BOOST_CHECK(stream.is_equal(expectedResult));
	}
BOOST_AUTO_TEST_SUITE_END()