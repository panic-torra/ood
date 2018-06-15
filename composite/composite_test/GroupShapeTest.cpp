#include "stdafx.h"

#include "../composite/GroupShape.h";
#include "../composite/Ellipse.h"
#include "../composite/Rectangle.h"

using namespace std;

struct Group_shape_
{
	shared_ptr<CGroupShape> groupShape = make_shared<CGroupShape>();
};

BOOST_FIXTURE_TEST_SUITE(Group_shape, Group_shape_)
	BOOST_AUTO_TEST_SUITE(when_created)
		BOOST_AUTO_TEST_CASE(is_empty)
		{
			BOOST_CHECK_EQUAL(groupShape->GetShapesCount(), 0);
		}

		BOOST_AUTO_TEST_CASE(has_empty_frame)
		{
			RectangleDouble emptyFrame = { 0, 0, 0, 0 };
			BOOST_CHECK_EQUAL(groupShape->GetFrame().left, emptyFrame.left);
			BOOST_CHECK_EQUAL(groupShape->GetFrame().top, emptyFrame.top);
			BOOST_CHECK_EQUAL(groupShape->GetFrame().width, emptyFrame.width);
			BOOST_CHECK_EQUAL(groupShape->GetFrame().height, emptyFrame.height);
		}

		BOOST_AUTO_TEST_CASE(throws_ecception_when_trying_to_get_a_shape)
		{
			BOOST_CHECK_THROW(groupShape->GetShapeAtIndex(1), out_of_range);
		}

		BOOST_AUTO_TEST_CASE(throws_ecception_when_trying_to_erase_a_shape)
		{
			BOOST_CHECK_THROW(groupShape->RemoveShapeAtIndex(1), out_of_range);
		}

		BOOST_AUTO_TEST_CASE(can_be_added_with_shape)
		{
			shared_ptr<CEllipse> ellipse = make_shared<CEllipse>(Vertex{ 300, 70 }, 30, 30);
			groupShape->InsertShape(ellipse);

			BOOST_CHECK_EQUAL(groupShape->GetShapesCount(), 1);
		}

		BOOST_AUTO_TEST_CASE(will_not_change_its_frame_if_we_will_try_to)
		{
			RectangleDouble newFrame = { 500, 50, 340, 220 };
			groupShape->SetFrame(newFrame);

			RectangleDouble emptyFrame = { 0, 0, 0, 0 };
			BOOST_CHECK_EQUAL(groupShape->GetFrame().left, emptyFrame.left);
			BOOST_CHECK_EQUAL(groupShape->GetFrame().top, emptyFrame.top);
			BOOST_CHECK_EQUAL(groupShape->GetFrame().width, emptyFrame.width);
			BOOST_CHECK_EQUAL(groupShape->GetFrame().height, emptyFrame.height);
		}

		BOOST_AUTO_TEST_CASE(will_return_empty_common_style_while_empty)
		{
			shared_ptr<const IStyle> commonFillStyle = groupShape->GetFillStyle();
			shared_ptr<const IStyle> commonOutlineStyle = groupShape->GetOutlineStyle();

			BOOST_CHECK(!commonFillStyle->GetColor());
			BOOST_CHECK(!commonOutlineStyle->GetColor());
		}

		BOOST_AUTO_TEST_CASE(will_return_common_style_if_got_shapes_with_same_style)
		{
			shared_ptr<CRectangle> rectangle = make_shared<CRectangle>(Vertex{ 300, 70 }, Vertex{ 400, 100 });
			auto rectangleStyle = rectangle->GetFillStyle();
			rectangleStyle->SetColor(127);

			shared_ptr<CEllipse> ellipse = make_shared<CEllipse>(Vertex{ 300, 70 }, 30, 30);
			auto ellipseStyle = ellipse->GetFillStyle();
			ellipseStyle->SetColor(127);

			groupShape->InsertShape(rectangle);
			groupShape->InsertShape(ellipse);

			shared_ptr<const IStyle> commonStyle = groupShape->GetFillStyle();

			BOOST_CHECK_EQUAL(commonStyle->IsEnabled(), true);
			BOOST_CHECK_EQUAL(commonStyle->GetColor(), (RGBAColor)127);
		}

		BOOST_AUTO_TEST_CASE(will_return_style_without_color_if_got_shapes_with_different_style)
		{
			shared_ptr<CRectangle> rectangle = make_shared<CRectangle>(Vertex{ 300, 70 }, Vertex{ 400, 100 });
			auto rectangleStyle = rectangle->GetFillStyle();
			rectangleStyle->SetColor(127);

			shared_ptr<CEllipse> ellipse = make_shared<CEllipse>(Vertex{ 300, 70 }, 30, 30);
			auto ellipseStyle = ellipse->GetFillStyle();
			ellipseStyle->SetColor(134);

			groupShape->InsertShape(rectangle);
			groupShape->InsertShape(ellipse);

			shared_ptr<IStyle> commonStyle = groupShape->GetFillStyle();
			BOOST_CHECK_EQUAL(commonStyle->GetColor(), boost::none);
		}

		struct after_adding_a_shape_ : Group_shape_
		{
			shared_ptr<CEllipse> ellipse = make_shared<CEllipse>(Vertex{ 300, 70 }, 30, 30);

			after_adding_a_shape_()
			{
				groupShape->InsertShape(ellipse);
			}
		};

		BOOST_FIXTURE_TEST_SUITE(after_adding_a_shape, after_adding_a_shape_)
			BOOST_AUTO_TEST_CASE(will_be_empty_after_erasing_shape)
			{
				groupShape->RemoveShapeAtIndex(0);
				BOOST_CHECK_EQUAL(groupShape->GetShapesCount(), 0);
			}

			BOOST_AUTO_TEST_CASE(will_have_same_frame_as_inner_shape)
			{
				RectangleDouble ellipseFrame = ellipse->GetFrame();

				BOOST_CHECK_EQUAL(groupShape->GetFrame().left, ellipseFrame.left);
				BOOST_CHECK_EQUAL(groupShape->GetFrame().top, ellipseFrame.top);
				BOOST_CHECK_EQUAL(groupShape->GetFrame().width, ellipseFrame.width);
				BOOST_CHECK_EQUAL(groupShape->GetFrame().height, ellipseFrame.height);
			}

			BOOST_AUTO_TEST_CASE(will_have_right_count_of_shapes_after_adding_one)
			{
				shared_ptr<CEllipse> newEllipse = make_shared<CEllipse>(Vertex{ 100, 50 }, 10, 10);
				groupShape->InsertShape(newEllipse);

				BOOST_CHECK_EQUAL(groupShape->GetShapesCount(), 2);
			}

			BOOST_AUTO_TEST_CASE(will_change_frame_of_inner_shape_after_changing_its_own_frame)
			{
				RectangleDouble newFrame = { 500, 50, 340, 220 };
				groupShape->SetFrame(newFrame);

				BOOST_CHECK_EQUAL(groupShape->GetFrame().left, newFrame.left);
				BOOST_CHECK_EQUAL(groupShape->GetFrame().top, newFrame.top);
				BOOST_CHECK_EQUAL(groupShape->GetFrame().width, newFrame.width);
				BOOST_CHECK_EQUAL(groupShape->GetFrame().height, newFrame.height);

				RectangleDouble ellipseFrame = ellipse->GetFrame();
				BOOST_CHECK_EQUAL(ellipseFrame.left, newFrame.left);
				BOOST_CHECK_EQUAL(ellipseFrame.top, newFrame.top);
				BOOST_CHECK_EQUAL(ellipseFrame.width, newFrame.width);
				BOOST_CHECK_EQUAL(ellipseFrame.height, newFrame.height);
			}

			BOOST_AUTO_TEST_CASE(will_change_its_frame_after_adding_shape)
			{
				RectangleDouble basicShape = { 270, 40, 60, 60 };

				BOOST_CHECK_EQUAL(groupShape->GetFrame().left, basicShape.left);
				BOOST_CHECK_EQUAL(groupShape->GetFrame().top, basicShape.top);
				BOOST_CHECK_EQUAL(groupShape->GetFrame().width, basicShape.width);
				BOOST_CHECK_EQUAL(groupShape->GetFrame().height, basicShape.height);

				shared_ptr<CEllipse> newEllipse = make_shared<CEllipse>(Vertex{ 100, 50 }, 10, 10);
				groupShape->InsertShape(newEllipse);
				RectangleDouble expectedShape = { 90, 40, 240, 60 };

				BOOST_CHECK_EQUAL(groupShape->GetFrame().left, expectedShape.left);
				BOOST_CHECK_EQUAL(groupShape->GetFrame().top, expectedShape.top);
				BOOST_CHECK_EQUAL(groupShape->GetFrame().width, expectedShape.width);
				BOOST_CHECK_EQUAL(groupShape->GetFrame().height, expectedShape.height);
			}
		BOOST_AUTO_TEST_SUITE_END()
	BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()