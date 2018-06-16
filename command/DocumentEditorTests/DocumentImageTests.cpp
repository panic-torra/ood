#include "stdafx.h"
#include <iostream>
#include "../DocumentEditor/Document.h"
#include "../DocumentEditor/FileUtils.h"

struct EmptyDocumentFixture
{
	CDocument document;
	const std::string DEFAULT_DOCUMENT_TITLE = "Untitled document";
	const std::string TEST_IMAGE_PATH = "test.jpg";
};

struct DocumentWithInsertedImage : EmptyDocumentFixture
{
	DocumentWithInsertedImage()
	{
		document.InsertImage(TEST_IMAGE_PATH, 1024, 768);
	}
};

struct DocumentWithInsertedImageAfterUndo : DocumentWithInsertedImage
{
	boost::filesystem::path tmpImagePath;
	DocumentWithInsertedImageAfterUndo()
	{
		tmpImagePath = document.GetItem(0)->GetImage()->GetPath();
		document.Undo();
	}
};

struct DocumentWithInsertedImageAfterUndoAndRedo : DocumentWithInsertedImageAfterUndo
{
	DocumentWithInsertedImageAfterUndoAndRedo()
	{
		document.Redo();
	}
};

struct DocumentWithInsertedImageAfterUndoDeletedFromHistory : DocumentWithInsertedImageAfterUndo
{
	DocumentWithInsertedImageAfterUndoDeletedFromHistory()
	{
		document.SetTitle("Test");
	}
};

struct DocumentWithResizedImage : DocumentWithInsertedImage
{
	std::shared_ptr<IImage> image;
	DocumentWithResizedImage()
	{
		image = document.GetItem(0)->GetImage();
		image->Resize(320u, 240u);
	}
};

struct DocumentWithResizedImageAfterUndo : DocumentWithResizedImage
{
	DocumentWithResizedImageAfterUndo()
	{
		document.Undo();
	}
};

struct DocumentWithResizedImageAfterUndoAndRedo : DocumentWithResizedImageAfterUndo
{
	DocumentWithResizedImageAfterUndoAndRedo()
	{
		document.Redo();
	}
};

struct DocumentWithResizedImageAfterUndoDeletedFromHistory : DocumentWithResizedImageAfterUndo
{
	DocumentWithResizedImageAfterUndoDeletedFromHistory()
	{
		document.SetTitle("Test");
	}
};

BOOST_AUTO_TEST_SUITE(Document)
	BOOST_FIXTURE_TEST_SUITE(when_image_is_inserted, DocumentWithInsertedImage)
		BOOST_AUTO_TEST_CASE(returns_image_temp_path)
		{
			auto image = document.GetItem(0)->GetImage();
			BOOST_CHECK(image != nullptr);
			BOOST_CHECK(image->GetPath() != TEST_IMAGE_PATH);
		}

		BOOST_AUTO_TEST_CASE(has_image_resource)
		{
			auto image = document.GetItem(0)->GetImage();
			BOOST_CHECK(image != nullptr);
			FileUtils::Exists(image->GetPath());
		}

		BOOST_AUTO_TEST_CASE(can_insert_image)
		{
			auto image = document.InsertImage(TEST_IMAGE_PATH, 640u, 480u);
			auto image1 = document.GetItem(0)->GetImage();
			BOOST_CHECK_EQUAL(image1->GetWidth(), 1024u);
			BOOST_CHECK_EQUAL(image1->GetHeight(), 768u);
			BOOST_CHECK(document.GetItem(1)->GetImage() == image);
		}

		BOOST_AUTO_TEST_CASE(returns_one_items_count)
		{
			BOOST_CHECK_EQUAL(document.GetItemsCount(), 1);
		}

		BOOST_AUTO_TEST_CASE(can_get_item)
		{
			BOOST_CHECK_NO_THROW(IDocumentItem::ConstPtr constCorrectPtr = document.GetItem(0));
			BOOST_CHECK_NO_THROW(IDocumentItem::Ptr correctPtr = document.GetItem(0));
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

		BOOST_FIXTURE_TEST_SUITE(after_undo, DocumentWithInsertedImageAfterUndo)
			BOOST_AUTO_TEST_CASE(keeps_image_resource)
			{
				BOOST_CHECK(FileUtils::Exists(tmpImagePath));
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

			BOOST_FIXTURE_TEST_SUITE(after_redo, DocumentWithInsertedImageAfterUndoAndRedo)
				BOOST_AUTO_TEST_CASE(keeps_image_resource)
				{
					BOOST_CHECK(FileUtils::Exists(tmpImagePath));
				}

				BOOST_AUTO_TEST_CASE(can_insert_image)
				{
					auto image = document.InsertImage(TEST_IMAGE_PATH, 640u, 480u);
					auto image1 = document.GetItem(0)->GetImage();
					BOOST_CHECK_EQUAL(image1->GetWidth(), 1024u);
					BOOST_CHECK_EQUAL(image1->GetHeight(), 768u);
					BOOST_CHECK(document.GetItem(1)->GetImage() == image);
				}

				BOOST_AUTO_TEST_CASE(returns_one_items_count)
				{
					BOOST_CHECK_EQUAL(document.GetItemsCount(), 1);
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

		BOOST_FIXTURE_TEST_SUITE(when_insert_image_is_deleted_from_history, DocumentWithInsertedImageAfterUndoDeletedFromHistory)
			BOOST_AUTO_TEST_CASE(removes_image_resource)
			{
				BOOST_CHECK(!FileUtils::Exists(tmpImagePath));
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

	BOOST_FIXTURE_TEST_SUITE(when_image_is_resized, DocumentWithResizedImage)
		BOOST_AUTO_TEST_CASE(returns_latest_size)
		{
			BOOST_CHECK_EQUAL(image->GetWidth(), 320u);
			BOOST_CHECK_EQUAL(image->GetHeight(), 240u);
		}

		BOOST_AUTO_TEST_CASE(can_delete_item)
		{
			document.DeleteItem(0);
			BOOST_CHECK_EQUAL(document.GetItemsCount(), 0);

			document.Undo();
			BOOST_CHECK_EQUAL(image->GetWidth(), 320u);
			BOOST_CHECK_EQUAL(image->GetHeight(), 240u);
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

		BOOST_FIXTURE_TEST_SUITE(after_undo, DocumentWithResizedImageAfterUndo)
			BOOST_AUTO_TEST_CASE(returns_previous_size)
			{
				BOOST_CHECK_EQUAL(image->GetWidth(), 1024u);
				BOOST_CHECK_EQUAL(image->GetHeight(), 768u);
			}

			BOOST_AUTO_TEST_CASE(can_delete_item)
			{
				document.DeleteItem(0);
				BOOST_CHECK_EQUAL(document.GetItemsCount(), 0);

				document.Undo();
				BOOST_CHECK_EQUAL(document.GetItemsCount(), 1);
			}

			BOOST_AUTO_TEST_CASE(can_undo)
			{
				document.Undo();
				BOOST_CHECK_EQUAL(document.GetItemsCount(), 0);
			}

			BOOST_AUTO_TEST_CASE(can_redo)
			{
				document.Redo();

				BOOST_CHECK_EQUAL(image->GetWidth(), 320u);
				BOOST_CHECK_EQUAL(image->GetHeight(), 240u);

				auto imageFromDocument = document.GetItem(0)->GetImage();
				BOOST_CHECK_EQUAL(imageFromDocument->GetWidth(), 320u);
				BOOST_CHECK_EQUAL(imageFromDocument->GetHeight(), 240u);
			}

			BOOST_FIXTURE_TEST_SUITE(after_redo, DocumentWithResizedImageAfterUndoAndRedo)
				BOOST_AUTO_TEST_CASE(returns_latest_size)
				{
					BOOST_CHECK_EQUAL(image->GetWidth(), 320u);
					BOOST_CHECK_EQUAL(image->GetHeight(), 240u);
				}

				BOOST_AUTO_TEST_CASE(can_delete_item)
				{
					document.DeleteItem(0);
					BOOST_CHECK_EQUAL(document.GetItemsCount(), 0);

					document.Undo();
					BOOST_CHECK_EQUAL(image->GetWidth(), 320u);
					BOOST_CHECK_EQUAL(image->GetHeight(), 240u);
				}

				BOOST_AUTO_TEST_CASE(can_undo)
				{
					BOOST_CHECK(document.CanUndo());
					BOOST_CHECK_NO_THROW(document.Undo());

				}

				BOOST_AUTO_TEST_CASE(cant_redo)
				{
					BOOST_CHECK_THROW(document.Redo(), std::runtime_error);
				}
			BOOST_AUTO_TEST_SUITE_END()

			BOOST_FIXTURE_TEST_SUITE(when_image_resize_is_deleted_from_history, DocumentWithResizedImageAfterUndoDeletedFromHistory)
				BOOST_AUTO_TEST_CASE(returns_previous_size)
				{
					BOOST_CHECK_EQUAL(image->GetWidth(), 1024u);
					BOOST_CHECK_EQUAL(image->GetHeight(), 768u);

					auto imageFromDocument = document.GetItem(0)->GetImage();
					BOOST_CHECK_EQUAL(imageFromDocument->GetWidth(), 1024u);
					BOOST_CHECK_EQUAL(imageFromDocument->GetHeight(), 768u);
				}

				BOOST_AUTO_TEST_CASE(keeps_image_resource)
				{
					BOOST_CHECK(FileUtils::Exists(image->GetPath()));
				}
			BOOST_AUTO_TEST_SUITE_END()
		BOOST_AUTO_TEST_SUITE_END()
	BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()