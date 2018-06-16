#include "stdafx.h"
#include "../DocumentEditor/Document.h"
#include "../DocumentEditor/FileUtils.h"

struct EmptyDocumentFixture
{
	CDocument document;
	const std::string DEFAULT_DOCUMENT_TITLE = "Untitled document";
	const std::string TEST_IMAGE_PATH = "test.jpg";
};

struct NotEmptyDocumentFixtureAtEndOfHistory : EmptyDocumentFixture
{
	std::shared_ptr<IParagraph> par1;
	std::shared_ptr<IImage> img1;
	std::shared_ptr<IParagraph> par2;
	std::shared_ptr<IImage> img2;

	NotEmptyDocumentFixtureAtEndOfHistory()
	{
		par1 = document.InsertParagraph("Test");
		img1 = document.InsertImage(TEST_IMAGE_PATH, 640u, 480u);
		document.SetTitle("Title");
		par2 = document.InsertParagraph("Test1");
		img2 = document.InsertImage(TEST_IMAGE_PATH, 1024u, 768u);

		document.SetTitle("Title1");
		par1->SetText("NewTestText");
		img1->Resize(69u, 42u);
	}
};

struct NotEmptyDocumentFixtureAtCenterOfHistory : NotEmptyDocumentFixtureAtEndOfHistory
{

	NotEmptyDocumentFixtureAtCenterOfHistory()
	{
		document.Undo();
		document.Undo();
		document.Undo();
		document.Undo();
		document.Undo();
	}
};

struct NotEmptyDocumentFixtureAtBeginOfHistory : NotEmptyDocumentFixtureAtCenterOfHistory
{
	NotEmptyDocumentFixtureAtBeginOfHistory()
	{
		document.Undo();
		document.Undo();
		document.Undo();
	}
};

BOOST_AUTO_TEST_SUITE(Document)
	BOOST_FIXTURE_TEST_SUITE(when_created, EmptyDocumentFixture)
		BOOST_AUTO_TEST_CASE(returns_zero_items_count)
		{
			BOOST_CHECK_EQUAL(document.GetItemsCount(), 0);
		}

		BOOST_AUTO_TEST_CASE(cant_get_item)
		{
			BOOST_CHECK_THROW(document.GetItem(0), std::runtime_error);
			BOOST_CHECK_THROW(document.GetItem(1), std::runtime_error);
			BOOST_CHECK_THROW(document.GetItem(2), std::runtime_error);
		}

		BOOST_AUTO_TEST_CASE(cant_delete_item)
		{
			BOOST_CHECK_THROW(document.DeleteItem(0), std::runtime_error);
			BOOST_CHECK_THROW(document.DeleteItem(1), std::runtime_error);
			BOOST_CHECK_THROW(document.DeleteItem(2), std::runtime_error);
		}

		BOOST_AUTO_TEST_CASE(has_default_title)
		{
			BOOST_CHECK_EQUAL(document.GetTitle(), DEFAULT_DOCUMENT_TITLE);
		}

		BOOST_AUTO_TEST_CASE(returns_false_when_check_can_undo)
		{
			BOOST_CHECK(!document.CanUndo());
		}

		BOOST_AUTO_TEST_CASE(cant_undo)
		{
			BOOST_CHECK_THROW(document.Undo(), std::runtime_error);
		}

		BOOST_AUTO_TEST_CASE(returns_false_when_check_can_redo)
		{
			BOOST_CHECK(!document.CanRedo());
		}

		BOOST_AUTO_TEST_CASE(cant_redo)
		{
			BOOST_CHECK_THROW(document.Redo(), std::runtime_error);
		}
	BOOST_AUTO_TEST_SUITE_END()

	BOOST_FIXTURE_TEST_SUITE(when_is_not_empty, NotEmptyDocumentFixtureAtCenterOfHistory)
		BOOST_AUTO_TEST_CASE(can_get_item)
		{
			BOOST_CHECK_NO_THROW(IDocumentItem::ConstPtr constCorrectPtr = document.GetItem(0));
			BOOST_CHECK_NO_THROW(IDocumentItem::Ptr correctPtr = document.GetItem(0));
		}

		BOOST_AUTO_TEST_CASE(allows_to_insert_paragraph_to_start)
		{
			const std::string TEXT = "Some unique paragraph text";
			BOOST_CHECK_NO_THROW(document.InsertParagraph(TEXT, 0));
			BOOST_CHECK_EQUAL(document.GetItemsCount(), 3);
			BOOST_CHECK_EQUAL(document.GetItem(0)->GetParagraph()->GetText(), TEXT);
		}

		BOOST_AUTO_TEST_CASE(allows_to_insert_image_to_start)
		{
			BOOST_CHECK_NO_THROW(document.InsertImage(TEST_IMAGE_PATH, 42, 2, 0));
			BOOST_CHECK_EQUAL(document.GetItemsCount(), 3);
			BOOST_CHECK_EQUAL(document.GetItem(0)->GetImage()->GetWidth(), 42);
		}

		BOOST_AUTO_TEST_CASE(allows_to_insert_paragraph_to_center)
		{
			const std::string TEXT = "Some unique paragraph text";
			BOOST_CHECK_NO_THROW(document.InsertParagraph(TEXT, 1));
			BOOST_CHECK_EQUAL(document.GetItemsCount(), 3);
			BOOST_CHECK_EQUAL(document.GetItem(1)->GetParagraph()->GetText(), TEXT);
		}

		BOOST_AUTO_TEST_CASE(allows_to_insert_image_to_center)
		{
			BOOST_CHECK_NO_THROW(document.InsertImage(TEST_IMAGE_PATH, 42, 2, 1));
			BOOST_CHECK_EQUAL(document.GetItemsCount(), 3);
			BOOST_CHECK_EQUAL(document.GetItem(1)->GetImage()->GetWidth(), 42);
		}

		BOOST_AUTO_TEST_CASE(allows_to_insert_paragraph_at_back)
		{
			const std::string TEXT = "Some unique paragraph text";
			BOOST_CHECK_NO_THROW(document.InsertParagraph(TEXT, document.GetItemsCount()));
			BOOST_CHECK_EQUAL(document.GetItemsCount(), 3);
			BOOST_CHECK_EQUAL(document.GetItem(document.GetItemsCount() - 1)->GetParagraph()->GetText(), TEXT);
		}

		BOOST_AUTO_TEST_CASE(allows_to_insert_image_at_back)
		{
			BOOST_CHECK_NO_THROW(document.InsertImage(TEST_IMAGE_PATH, 42, 2, document.GetItemsCount()));
			BOOST_CHECK_EQUAL(document.GetItemsCount(), 3);
			BOOST_CHECK_EQUAL(document.GetItem(document.GetItemsCount() - 1)->GetImage()->GetWidth(), 42);
		}

		BOOST_AUTO_TEST_CASE(throws_error_when_trying_to_insert_paragraph_out_of_range)
		{
			BOOST_CHECK_THROW(document.InsertParagraph("Test", document.GetItemsCount() + 1), std::runtime_error);
			BOOST_CHECK_THROW(document.InsertParagraph("Test", document.GetItemsCount() + 2), std::runtime_error);
			BOOST_CHECK_EQUAL(document.GetItemsCount(), 2);
		}

		BOOST_AUTO_TEST_CASE(throws_error_when_trying_to_insert_image_out_of_range)
		{
			BOOST_CHECK_THROW(document.InsertImage(TEST_IMAGE_PATH, 1, 1, document.GetItemsCount() + 1), std::runtime_error);
			BOOST_CHECK_THROW(document.InsertImage(TEST_IMAGE_PATH, 1, 1, document.GetItemsCount() + 2), std::runtime_error);
			BOOST_CHECK_EQUAL(document.GetItemsCount(), 2);
		}

		BOOST_FIXTURE_TEST_CASE(can_move_its_history_from_end_to_beginning, NotEmptyDocumentFixtureAtEndOfHistory)
		{
			BOOST_CHECK_EQUAL(img1->GetWidth(), 69u);
			BOOST_CHECK_EQUAL(img1->GetHeight(), 42u);
			BOOST_CHECK_EQUAL(img2->GetWidth(), 1024u);
			BOOST_CHECK_EQUAL(img2->GetHeight(), 768u);
			BOOST_CHECK_EQUAL(par1->GetText(), "NewTestText");
			BOOST_CHECK_EQUAL(par2->GetText(), "Test1");
			BOOST_CHECK_EQUAL(document.GetTitle(), "Title1");
			BOOST_CHECK_EQUAL(document.GetItemsCount(), 4);
			BOOST_CHECK(!document.CanRedo());
			BOOST_CHECK(document.CanUndo());

			document.Undo();
			BOOST_CHECK_EQUAL(img1->GetWidth(), 640u);
			BOOST_CHECK_EQUAL(img1->GetHeight(), 480u);
			BOOST_CHECK_EQUAL(img2->GetWidth(), 1024u);
			BOOST_CHECK_EQUAL(img2->GetHeight(), 768u);
			BOOST_CHECK_EQUAL(par1->GetText(), "NewTestText");
			BOOST_CHECK_EQUAL(par2->GetText(), "Test1");
			BOOST_CHECK_EQUAL(document.GetTitle(), "Title1");
			BOOST_CHECK_EQUAL(document.GetItemsCount(), 4);
			BOOST_CHECK(document.CanRedo());
			BOOST_CHECK(document.CanUndo());

			document.Undo();
			BOOST_CHECK_EQUAL(img1->GetWidth(), 640u);
			BOOST_CHECK_EQUAL(img1->GetHeight(), 480u);
			BOOST_CHECK_EQUAL(img2->GetWidth(), 1024u);
			BOOST_CHECK_EQUAL(img2->GetHeight(), 768u);
			BOOST_CHECK_EQUAL(par1->GetText(), "Test");
			BOOST_CHECK_EQUAL(par2->GetText(), "Test1");
			BOOST_CHECK_EQUAL(document.GetTitle(), "Title1");
			BOOST_CHECK_EQUAL(document.GetItemsCount(), 4);
			BOOST_CHECK(document.CanRedo());
			BOOST_CHECK(document.CanUndo());

			document.Undo();
			BOOST_CHECK_EQUAL(img1->GetWidth(), 640u);
			BOOST_CHECK_EQUAL(img1->GetHeight(), 480u);
			BOOST_CHECK_EQUAL(img2->GetWidth(), 1024u);
			BOOST_CHECK_EQUAL(img2->GetHeight(), 768u);
			BOOST_CHECK_EQUAL(par1->GetText(), "Test");
			BOOST_CHECK_EQUAL(par2->GetText(), "Test1");
			BOOST_CHECK_EQUAL(document.GetTitle(), "Title");
			BOOST_CHECK_EQUAL(document.GetItemsCount(), 4);
			BOOST_CHECK(document.CanRedo());
			BOOST_CHECK(document.CanUndo());

			document.Undo();
			BOOST_CHECK_EQUAL(img1->GetWidth(), 640u);
			BOOST_CHECK_EQUAL(img1->GetHeight(), 480u);
			BOOST_CHECK_EQUAL(img2->GetWidth(), 1024u);
			BOOST_CHECK_EQUAL(img2->GetHeight(), 768u);
			BOOST_CHECK_EQUAL(par1->GetText(), "Test");
			BOOST_CHECK_EQUAL(par2->GetText(), "Test1");
			BOOST_CHECK_EQUAL(document.GetTitle(), "Title");
			BOOST_CHECK_EQUAL(document.GetItemsCount(), 3);
			BOOST_CHECK(document.CanRedo());
			BOOST_CHECK(document.CanUndo());

			document.Undo();
			BOOST_CHECK_EQUAL(img1->GetWidth(), 640u);
			BOOST_CHECK_EQUAL(img1->GetHeight(), 480u);
			BOOST_CHECK_EQUAL(img2->GetWidth(), 1024u);
			BOOST_CHECK_EQUAL(img2->GetHeight(), 768u);
			BOOST_CHECK_EQUAL(par1->GetText(), "Test");
			BOOST_CHECK_EQUAL(par2->GetText(), "Test1");
			BOOST_CHECK_EQUAL(document.GetTitle(), "Title");
			BOOST_CHECK_EQUAL(document.GetItemsCount(), 2);
			BOOST_CHECK(document.CanRedo());
			BOOST_CHECK(document.CanUndo());

			document.Undo();
			BOOST_CHECK_EQUAL(img1->GetWidth(), 640u);
			BOOST_CHECK_EQUAL(img1->GetHeight(), 480u);
			BOOST_CHECK_EQUAL(img2->GetWidth(), 1024u);
			BOOST_CHECK_EQUAL(img2->GetHeight(), 768u);
			BOOST_CHECK_EQUAL(par1->GetText(), "Test");
			BOOST_CHECK_EQUAL(par2->GetText(), "Test1");
			BOOST_CHECK_EQUAL(document.GetTitle(), DEFAULT_DOCUMENT_TITLE);
			BOOST_CHECK_EQUAL(document.GetItemsCount(), 2);
			BOOST_CHECK(document.CanRedo());
			BOOST_CHECK(document.CanUndo());

			document.Undo();
			BOOST_CHECK_EQUAL(img1->GetWidth(), 640u);
			BOOST_CHECK_EQUAL(img1->GetHeight(), 480u);
			BOOST_CHECK_EQUAL(img2->GetWidth(), 1024u);
			BOOST_CHECK_EQUAL(img2->GetHeight(), 768u);
			BOOST_CHECK_EQUAL(par1->GetText(), "Test");
			BOOST_CHECK_EQUAL(par2->GetText(), "Test1");
			BOOST_CHECK_EQUAL(document.GetTitle(), DEFAULT_DOCUMENT_TITLE);
			BOOST_CHECK_EQUAL(document.GetItemsCount(), 1);
			BOOST_CHECK(document.CanRedo());
			BOOST_CHECK(document.CanUndo());

			document.Undo();
			BOOST_CHECK_EQUAL(img1->GetWidth(), 640u);
			BOOST_CHECK_EQUAL(img1->GetHeight(), 480u);
			BOOST_CHECK_EQUAL(img2->GetWidth(), 1024u);
			BOOST_CHECK_EQUAL(img2->GetHeight(), 768u);
			BOOST_CHECK_EQUAL(par1->GetText(), "Test");
			BOOST_CHECK_EQUAL(par2->GetText(), "Test1");
			BOOST_CHECK_EQUAL(document.GetTitle(), DEFAULT_DOCUMENT_TITLE);
			BOOST_CHECK_EQUAL(document.GetItemsCount(), 0);
			BOOST_CHECK(document.CanRedo());
			BOOST_CHECK(!document.CanUndo());
		}

		BOOST_FIXTURE_TEST_CASE(can_move_its_history_from_beginning_to_end, NotEmptyDocumentFixtureAtBeginOfHistory)
		{
			BOOST_CHECK_EQUAL(img1->GetWidth(), 640u);
			BOOST_CHECK_EQUAL(img1->GetHeight(), 480u);
			BOOST_CHECK_EQUAL(img2->GetWidth(), 1024u);
			BOOST_CHECK_EQUAL(img2->GetHeight(), 768u);
			BOOST_CHECK_EQUAL(par1->GetText(), "Test");
			BOOST_CHECK_EQUAL(par2->GetText(), "Test1");
			BOOST_CHECK_EQUAL(document.GetTitle(), DEFAULT_DOCUMENT_TITLE);
			BOOST_CHECK_EQUAL(document.GetItemsCount(), 0);
			BOOST_CHECK(document.CanRedo());
			BOOST_CHECK(!document.CanUndo());

			document.Redo();
			BOOST_CHECK_EQUAL(img1->GetWidth(), 640u);
			BOOST_CHECK_EQUAL(img1->GetHeight(), 480u);
			BOOST_CHECK_EQUAL(img2->GetWidth(), 1024u);
			BOOST_CHECK_EQUAL(img2->GetHeight(), 768u);
			BOOST_CHECK_EQUAL(par1->GetText(), "Test");
			BOOST_CHECK_EQUAL(par2->GetText(), "Test1");
			BOOST_CHECK_EQUAL(document.GetTitle(), DEFAULT_DOCUMENT_TITLE);
			BOOST_CHECK_EQUAL(document.GetItemsCount(), 1);
			BOOST_CHECK(document.CanRedo());
			BOOST_CHECK(document.CanUndo());

			document.Redo();
			BOOST_CHECK_EQUAL(img1->GetWidth(), 640u);
			BOOST_CHECK_EQUAL(img1->GetHeight(), 480u);
			BOOST_CHECK_EQUAL(img2->GetWidth(), 1024u);
			BOOST_CHECK_EQUAL(img2->GetHeight(), 768u);
			BOOST_CHECK_EQUAL(par1->GetText(), "Test");
			BOOST_CHECK_EQUAL(par2->GetText(), "Test1");
			BOOST_CHECK_EQUAL(document.GetTitle(), DEFAULT_DOCUMENT_TITLE);
			BOOST_CHECK_EQUAL(document.GetItemsCount(), 2);
			BOOST_CHECK(document.CanRedo());
			BOOST_CHECK(document.CanUndo());

			document.Redo();
			BOOST_CHECK_EQUAL(img1->GetWidth(), 640u);
			BOOST_CHECK_EQUAL(img1->GetHeight(), 480u);
			BOOST_CHECK_EQUAL(img2->GetWidth(), 1024u);
			BOOST_CHECK_EQUAL(img2->GetHeight(), 768u);
			BOOST_CHECK_EQUAL(par1->GetText(), "Test");
			BOOST_CHECK_EQUAL(par2->GetText(), "Test1");
			BOOST_CHECK_EQUAL(document.GetTitle(), "Title");
			BOOST_CHECK_EQUAL(document.GetItemsCount(), 2);
			BOOST_CHECK(document.CanRedo());
			BOOST_CHECK(document.CanUndo());

			document.Redo();
			BOOST_CHECK_EQUAL(img1->GetWidth(), 640u);
			BOOST_CHECK_EQUAL(img1->GetHeight(), 480u);
			BOOST_CHECK_EQUAL(img2->GetWidth(), 1024u);
			BOOST_CHECK_EQUAL(img2->GetHeight(), 768u);
			BOOST_CHECK_EQUAL(par1->GetText(), "Test");
			BOOST_CHECK_EQUAL(par2->GetText(), "Test1");
			BOOST_CHECK_EQUAL(document.GetTitle(), "Title");
			BOOST_CHECK_EQUAL(document.GetItemsCount(), 3);
			BOOST_CHECK(document.CanRedo());
			BOOST_CHECK(document.CanUndo());

			document.Redo();
			BOOST_CHECK_EQUAL(img1->GetWidth(), 640u);
			BOOST_CHECK_EQUAL(img1->GetHeight(), 480u);
			BOOST_CHECK_EQUAL(img2->GetWidth(), 1024u);
			BOOST_CHECK_EQUAL(img2->GetHeight(), 768u);
			BOOST_CHECK_EQUAL(par1->GetText(), "Test");
			BOOST_CHECK_EQUAL(par2->GetText(), "Test1");
			BOOST_CHECK_EQUAL(document.GetTitle(), "Title");
			BOOST_CHECK_EQUAL(document.GetItemsCount(), 4);
			BOOST_CHECK(document.CanRedo());
			BOOST_CHECK(document.CanUndo());

			document.Redo();
			BOOST_CHECK_EQUAL(img1->GetWidth(), 640u);
			BOOST_CHECK_EQUAL(img1->GetHeight(), 480u);
			BOOST_CHECK_EQUAL(img2->GetWidth(), 1024u);
			BOOST_CHECK_EQUAL(img2->GetHeight(), 768u);
			BOOST_CHECK_EQUAL(par1->GetText(), "Test");
			BOOST_CHECK_EQUAL(par2->GetText(), "Test1");
			BOOST_CHECK_EQUAL(document.GetTitle(), "Title1");
			BOOST_CHECK_EQUAL(document.GetItemsCount(), 4);
			BOOST_CHECK(document.CanRedo());
			BOOST_CHECK(document.CanUndo());

			document.Redo();
			BOOST_CHECK_EQUAL(img1->GetWidth(), 640u);
			BOOST_CHECK_EQUAL(img1->GetHeight(), 480u);
			BOOST_CHECK_EQUAL(img2->GetWidth(), 1024u);
			BOOST_CHECK_EQUAL(img2->GetHeight(), 768u);
			BOOST_CHECK_EQUAL(par1->GetText(), "NewTestText");
			BOOST_CHECK_EQUAL(par2->GetText(), "Test1");
			BOOST_CHECK_EQUAL(document.GetTitle(), "Title1");
			BOOST_CHECK_EQUAL(document.GetItemsCount(), 4);
			BOOST_CHECK(document.CanRedo());
			BOOST_CHECK(document.CanUndo());

			document.Redo();
			BOOST_CHECK_EQUAL(img1->GetWidth(), 69u);
			BOOST_CHECK_EQUAL(img1->GetHeight(), 42u);
			BOOST_CHECK_EQUAL(img2->GetWidth(), 1024u);
			BOOST_CHECK_EQUAL(img2->GetHeight(), 768u);
			BOOST_CHECK_EQUAL(par1->GetText(), "NewTestText");
			BOOST_CHECK_EQUAL(par2->GetText(), "Test1");
			BOOST_CHECK_EQUAL(document.GetTitle(), "Title1");
			BOOST_CHECK_EQUAL(document.GetItemsCount(), 4);
			BOOST_CHECK(!document.CanRedo());
			BOOST_CHECK(document.CanUndo());
		}

		BOOST_AUTO_TEST_CASE(returns_error_when_cant_insert_item)
		{
			BOOST_CHECK_THROW(document.InsertParagraph("Test", 666), std::runtime_error);
			BOOST_CHECK_THROW(document.InsertImage(TEST_IMAGE_PATH, 640u, 480u, 666), std::runtime_error);
			BOOST_CHECK_THROW(document.InsertImage("NotExistedImage", 640u, 480u), std::runtime_error);
			BOOST_CHECK_EQUAL(img1->GetWidth(), 640u);
			BOOST_CHECK_EQUAL(img1->GetHeight(), 480u);
			BOOST_CHECK_EQUAL(img2->GetWidth(), 1024u);
			BOOST_CHECK_EQUAL(img2->GetHeight(), 768u);
			BOOST_CHECK_EQUAL(par1->GetText(), "Test");
			BOOST_CHECK_EQUAL(par2->GetText(), "Test1");
			BOOST_CHECK_EQUAL(document.GetTitle(), "Title");
			BOOST_CHECK_EQUAL(document.GetItemsCount(), 2);
			BOOST_CHECK(document.CanRedo());
			BOOST_CHECK(document.CanUndo());
		}

		BOOST_AUTO_TEST_CASE(returns_error_when_cant_delete_item)
		{
			BOOST_CHECK_THROW(document.DeleteItem(666), std::runtime_error);
			BOOST_CHECK_EQUAL(img1->GetWidth(), 640u);
			BOOST_CHECK_EQUAL(img1->GetHeight(), 480u);
			BOOST_CHECK_EQUAL(img2->GetWidth(), 1024u);
			BOOST_CHECK_EQUAL(img2->GetHeight(), 768u);
			BOOST_CHECK_EQUAL(par1->GetText(), "Test");
			BOOST_CHECK_EQUAL(par2->GetText(), "Test1");
			BOOST_CHECK_EQUAL(document.GetTitle(), "Title");
			BOOST_CHECK_EQUAL(document.GetItemsCount(), 2);
			BOOST_CHECK(document.CanRedo());
			BOOST_CHECK(document.CanUndo());
		}

	BOOST_AUTO_TEST_SUITE_END()

	BOOST_AUTO_TEST_CASE(removes_temp_path_after_exit)
	{
		boost::filesystem::path tempPath;
		{
			CDocument document;
			document.InsertImage("test.jpg", 640u, 480u);
			tempPath = document.GetTempPath();
			BOOST_CHECK(FileUtils::Exists(tempPath));
		}
		BOOST_CHECK(!FileUtils::Exists(tempPath));
	}

BOOST_AUTO_TEST_SUITE_END()