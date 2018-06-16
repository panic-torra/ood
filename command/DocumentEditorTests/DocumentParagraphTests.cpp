#include "stdafx.h"
#include "../DocumentEditor/Document.h"
#include "../DocumentEditor/FileUtils.h"

struct EmptyDocumentFixture
{
	CDocument document;
	const std::string DEFAULT_DOCUMENT_TITLE = "Untitled document";
	const std::string TEST_IMAGE_PATH = "test.jpg";
};

struct DocumentWithInsertedParagraph : EmptyDocumentFixture
{
	const std::string PARAGRAPH_TEXT = "Some paragraph text";
	DocumentWithInsertedParagraph()
	{
		document.InsertParagraph(PARAGRAPH_TEXT);
	}
};

struct DocumentWithInsertedParagraphAfterUndo : DocumentWithInsertedParagraph
{
	DocumentWithInsertedParagraphAfterUndo()
	{
		document.Undo();
	}
};

struct DocumentWithInsertedParagraphAfterUndoAndRedo : DocumentWithInsertedParagraphAfterUndo
{
	DocumentWithInsertedParagraphAfterUndoAndRedo()
	{
		document.Redo();
	}
};

struct DocumentWithInsertedParagraphAfterUndoDeletedFromHistory : DocumentWithInsertedParagraphAfterUndo
{
	DocumentWithInsertedParagraphAfterUndoDeletedFromHistory()
	{
		document.SetTitle("Test"); // execute some command to delete undo`ed command from history
	}
};

struct DocumentWithChangedParagraphText : DocumentWithInsertedParagraph
{
	const std::string NEW_PARAGRAPH_TEXT = "New paragraph text!";

	std::shared_ptr<IParagraph> paragraph;
	DocumentWithChangedParagraphText()
	{
		paragraph = document.GetItem(0)->GetParagraph();
		paragraph->SetText(NEW_PARAGRAPH_TEXT);
	}
};

struct DocumentWithChangedParagraphTextAfterUndo : DocumentWithChangedParagraphText
{
	DocumentWithChangedParagraphTextAfterUndo()
	{
		document.Undo();
	}
};

struct DocumentWithChangedParagraphTextAfterUndoAndRedo : DocumentWithChangedParagraphTextAfterUndo
{
	DocumentWithChangedParagraphTextAfterUndoAndRedo()
	{
		document.Redo();
	}
};

struct DocumentWithChangedParagraphTextAfterUndoDeletedFromHistory : DocumentWithChangedParagraphTextAfterUndo
{
	DocumentWithChangedParagraphTextAfterUndoDeletedFromHistory()
	{
		document.SetTitle("Test"); // execute some command to delete undo`ed command from history
	}
};

BOOST_AUTO_TEST_SUITE(Document)
	BOOST_FIXTURE_TEST_SUITE(when_paragraph_is_inserted, DocumentWithInsertedParagraph)
		BOOST_AUTO_TEST_CASE(can_insert_paragraph)
		{
			auto paragraph = document.InsertParagraph("Test");
			BOOST_CHECK(document.GetItem(0)->GetParagraph()->GetText() == PARAGRAPH_TEXT);
			BOOST_CHECK(document.GetItem(1)->GetParagraph() == paragraph);
		}

		BOOST_AUTO_TEST_CASE(can_insert_image)
		{
			auto image = document.InsertImage(TEST_IMAGE_PATH, 640u, 480u);
			BOOST_CHECK(document.GetItem(0)->GetParagraph()->GetText() == PARAGRAPH_TEXT);
			BOOST_CHECK(document.GetItem(1)->GetImage() == image);
		}

		BOOST_AUTO_TEST_CASE(returns_one_items_count)
		{
			BOOST_CHECK_EQUAL(document.GetItemsCount(), 1);
		}

		BOOST_AUTO_TEST_CASE(can_get_item)
		{
			auto paragraph = document.GetItem(0)->GetParagraph();
			BOOST_CHECK(paragraph != nullptr);
			BOOST_CHECK_EQUAL(paragraph->GetText(), PARAGRAPH_TEXT);
		}

		BOOST_AUTO_TEST_CASE(cant_get_item_by_out_of_range_index)
		{
			BOOST_CHECK_THROW(document.GetItem(1), std::runtime_error);
			BOOST_CHECK_THROW(document.GetItem(2), std::runtime_error);
		}

		BOOST_AUTO_TEST_CASE(can_delete_item)
		{
			BOOST_CHECK_NO_THROW(document.DeleteItem(0));
			BOOST_CHECK_EQUAL(document.GetItemsCount(), 0);
			BOOST_CHECK_THROW(document.GetItem(0), std::runtime_error);
		}

		BOOST_AUTO_TEST_CASE(returns_true_when_check_can_undo)
		{
			BOOST_CHECK(document.CanUndo());
		}

		BOOST_AUTO_TEST_CASE(can_undo)
		{
			document.Undo();
			BOOST_CHECK_EQUAL(document.GetTitle(), DEFAULT_DOCUMENT_TITLE);
		}

		BOOST_AUTO_TEST_CASE(returns_false_when_check_can_redo)
		{
			BOOST_CHECK(!document.CanRedo());
		}

		BOOST_AUTO_TEST_CASE(cant_redo)
		{
			BOOST_CHECK_THROW(document.Redo(), std::runtime_error);
			BOOST_CHECK_EQUAL(document.GetItemsCount(), 1);
			BOOST_CHECK(document.CanUndo());
		}

		BOOST_FIXTURE_TEST_SUITE(after_undo, DocumentWithInsertedParagraphAfterUndo)
			BOOST_AUTO_TEST_CASE(can_insert_paragraph)
			{
				auto paragraph = document.InsertParagraph("Test");
				BOOST_CHECK(document.GetItem(0)->GetParagraph() == paragraph);
				BOOST_CHECK_EQUAL(document.GetItemsCount(), 1);
			}

			BOOST_AUTO_TEST_CASE(can_insert_image)
			{
				auto image = document.InsertImage(TEST_IMAGE_PATH, 640u, 480u);
				BOOST_CHECK(document.GetItem(0)->GetImage() == image);
				BOOST_CHECK_EQUAL(document.GetItemsCount(), 1);
			}

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
				BOOST_CHECK(document.CanRedo());
			}

			BOOST_AUTO_TEST_CASE(returns_false_when_check_can_undo)
			{
				BOOST_CHECK(!document.CanUndo());
			}

			BOOST_AUTO_TEST_CASE(cant_undo)
			{
				BOOST_CHECK_THROW(document.Undo(), std::runtime_error);
				BOOST_CHECK_EQUAL(document.GetItemsCount(), 0);
			}

			BOOST_AUTO_TEST_CASE(returns_true_when_check_can_redo)
			{
				BOOST_CHECK(document.CanRedo());
			}

			BOOST_AUTO_TEST_CASE(can_redo)
			{
				BOOST_CHECK_NO_THROW(document.Redo());
				BOOST_CHECK_EQUAL(document.GetItemsCount(), 1);
			}

			BOOST_FIXTURE_TEST_SUITE(after_redo, DocumentWithInsertedParagraphAfterUndoAndRedo)
				BOOST_AUTO_TEST_CASE(can_insert_paragraph)
				{
					auto paragraph = document.InsertParagraph("Test");
					BOOST_CHECK(document.GetItem(0)->GetParagraph()->GetText() == PARAGRAPH_TEXT);
					BOOST_CHECK(document.GetItem(1)->GetParagraph() == paragraph);
				}

				BOOST_AUTO_TEST_CASE(can_insert_image)
				{
					auto image = document.InsertImage(TEST_IMAGE_PATH, 640u, 480u);
					BOOST_CHECK(document.GetItem(0)->GetParagraph()->GetText() == PARAGRAPH_TEXT);
					BOOST_CHECK(document.GetItem(1)->GetImage() == image);
				}

				BOOST_AUTO_TEST_CASE(returns_one_items_count)
				{
					BOOST_CHECK_EQUAL(document.GetItemsCount(), 1);
				}

				BOOST_AUTO_TEST_CASE(can_get_item)
				{
					auto paragraph = document.GetItem(0)->GetParagraph();
					BOOST_CHECK(paragraph != nullptr);
					BOOST_CHECK_EQUAL(paragraph->GetText(), PARAGRAPH_TEXT);
				}

				BOOST_AUTO_TEST_CASE(cant_get_item_by_out_of_range_index)
				{
					BOOST_CHECK_THROW(document.GetItem(1), std::runtime_error);
					BOOST_CHECK_THROW(document.GetItem(2), std::runtime_error);
				}

				BOOST_AUTO_TEST_CASE(can_delete_item)
				{
					BOOST_CHECK_NO_THROW(document.DeleteItem(0));
					BOOST_CHECK_EQUAL(document.GetItemsCount(), 0);
					BOOST_CHECK_THROW(document.GetItem(0), std::runtime_error);
				}

				BOOST_AUTO_TEST_CASE(returns_true_when_check_can_undo)
				{
					BOOST_CHECK(document.CanUndo());
				}

				BOOST_AUTO_TEST_CASE(can_undo)
				{
					document.Undo();
					BOOST_CHECK_EQUAL(document.GetItemsCount(), 0);
				}

				BOOST_AUTO_TEST_CASE(returns_false_when_check_can_redo)
				{
					BOOST_CHECK(!document.CanRedo());
				}

				BOOST_AUTO_TEST_CASE(cant_redo)
				{
					BOOST_CHECK_THROW(document.Redo(), std::runtime_error);
					BOOST_CHECK_EQUAL(document.GetItemsCount(), 1);
					BOOST_CHECK(document.CanUndo());
				}

			BOOST_AUTO_TEST_SUITE_END()

			BOOST_FIXTURE_TEST_SUITE(when_insert_paragraph_is_deleted_from_history, DocumentWithInsertedParagraphAfterUndoDeletedFromHistory)
				BOOST_AUTO_TEST_CASE(can_insert_paragraph)
				{
					auto paragraph = document.InsertParagraph("Test");
					BOOST_CHECK(document.GetItem(0)->GetParagraph() == paragraph);
					BOOST_CHECK_EQUAL(document.GetItemsCount(), 1);
				}

				BOOST_AUTO_TEST_CASE(can_insert_image)
				{
					auto image = document.InsertImage(TEST_IMAGE_PATH, 640u, 480u);
					BOOST_CHECK(document.GetItem(0)->GetImage() == image);
					BOOST_CHECK_EQUAL(document.GetItemsCount(), 1);
				}

				BOOST_AUTO_TEST_CASE(can_undo_only_once)
				{
					BOOST_CHECK(document.CanUndo());
					document.Undo();

					BOOST_CHECK(!document.CanUndo());
					BOOST_CHECK_THROW(document.Undo(), std::runtime_error);
				}

				BOOST_AUTO_TEST_CASE(cant_redo)
				{
					BOOST_CHECK_THROW(document.Redo(), std::runtime_error);
				}
			BOOST_AUTO_TEST_SUITE_END()
		BOOST_AUTO_TEST_SUITE_END()
	BOOST_AUTO_TEST_SUITE_END()

	BOOST_FIXTURE_TEST_SUITE(when_paragraph_is_changed, DocumentWithChangedParagraphText)
		BOOST_AUTO_TEST_CASE(returns_latest_text)
		{
			BOOST_CHECK_EQUAL(paragraph->GetText(), NEW_PARAGRAPH_TEXT);
		}

		BOOST_AUTO_TEST_CASE(can_undo)
		{
			BOOST_CHECK(document.CanUndo());
			BOOST_CHECK_NO_THROW(document.Undo());
		}

		BOOST_AUTO_TEST_CASE(cant_redo)
		{
			BOOST_CHECK(!document.CanRedo());
			BOOST_CHECK_THROW(document.Redo(), std::runtime_error);
		}

		BOOST_FIXTURE_TEST_SUITE(after_undo, DocumentWithChangedParagraphTextAfterUndo)
			BOOST_AUTO_TEST_CASE(returns_previous_text)
			{
				BOOST_CHECK_EQUAL(paragraph->GetText(), PARAGRAPH_TEXT);
			}

			BOOST_AUTO_TEST_CASE(can_undo)
			{
				BOOST_CHECK(document.CanUndo());
				BOOST_CHECK_NO_THROW(document.Undo());
			}

			BOOST_AUTO_TEST_CASE(can_redo)
			{
				BOOST_CHECK(document.CanUndo());
				BOOST_CHECK_NO_THROW(document.Undo());
			}

			BOOST_FIXTURE_TEST_SUITE(after_redo, DocumentWithChangedParagraphTextAfterUndoAndRedo)
				BOOST_AUTO_TEST_CASE(returns_latest_text)
				{
					BOOST_CHECK_EQUAL(paragraph->GetText(), NEW_PARAGRAPH_TEXT);
				}

				BOOST_AUTO_TEST_CASE(can_undo)
				{
					BOOST_CHECK(document.CanUndo());
					BOOST_CHECK_NO_THROW(document.Undo());
				}

				BOOST_AUTO_TEST_CASE(cant_redo)
				{
					BOOST_CHECK(!document.CanRedo());
					BOOST_CHECK_THROW(document.Redo(), std::runtime_error);
				}
			BOOST_AUTO_TEST_SUITE_END()

			BOOST_FIXTURE_TEST_SUITE(when_image_resize_is_deleted_from_history, DocumentWithChangedParagraphTextAfterUndoDeletedFromHistory)
				BOOST_AUTO_TEST_CASE(returns_previous_text)
				{
					BOOST_CHECK_EQUAL(paragraph->GetText(), PARAGRAPH_TEXT);
				}
			BOOST_AUTO_TEST_SUITE_END()
		BOOST_AUTO_TEST_SUITE_END()
	BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()