#include "stdafx.h"
#include "../DocumentEditor/Document.h"
#include "../DocumentEditor/FileUtils.h"

struct DocumentFixture
{
	CDocument document;
	boost::filesystem::path testFilePath;
	boost::filesystem::path testFileFolderPath;
	boost::filesystem::path testImage;
	DocumentFixture()
		: testFileFolderPath("test"), testFilePath("test//test.html"), testImage("test.jpg")
	{
		boost::filesystem::remove_all(testFileFolderPath);
		FileUtils::CreateDirReqursively(testFileFolderPath);
	}
	~DocumentFixture()
	{
		try
		{
			boost::filesystem::remove_all(testFileFolderPath);
		}
		catch (...)
		{
		}
	}
	std::string GetResultString()
	{
		auto file = std::ifstream(testFilePath.string(), std::ios::app);
		BOOST_CHECK(file.is_open());

		std::string line;
		std::string result;
		while (std::getline(file, line))
		{
			result.append(line);
		}
		return result;
	}

	std::vector<boost::filesystem::path> GetFoldersByRegex(boost::filesystem::path const& target, boost::regex const& filter)
	{
		std::vector<boost::filesystem::path> matchingFolders;

		boost::filesystem::directory_iterator end_itr;
		for (boost::filesystem::directory_iterator i(target); i != end_itr; ++i)
		{
			if (!boost::filesystem::is_directory(i->status())) continue;

			boost::smatch what;

			if (!boost::regex_match(i->path().filename().string(), what, filter)) continue;

			matchingFolders.push_back(i->path());
		}
		return matchingFolders;
	}
};

struct AlreadyCreatedHtmlFileFixture : DocumentFixture
{
	const boost::filesystem::path createdFile;

	AlreadyCreatedHtmlFileFixture()
		: createdFile("test.html")
	{
		boost::filesystem::ofstream(createdFile.string());
	}

	~AlreadyCreatedHtmlFileFixture()
	{
		try
		{
			FileUtils::Delete(createdFile);
		}
		catch (...)
		{
		}
	}
};

struct NotEmptyDocumentFixture : DocumentFixture
{
	std::shared_ptr<IParagraph> par1;
	std::shared_ptr<IImage> img1;
	std::shared_ptr<IParagraph> par2;
	std::shared_ptr<IImage> img2;

	NotEmptyDocumentFixture()
	{
		par1 = document.InsertParagraph("Test");
		img1 = document.InsertImage(testImage, 640u, 480u);
		document.SetTitle("Title");
		par2 = document.InsertParagraph("Test1");
		img2 = document.InsertImage(testImage, 1024u, 768u);

		document.SetTitle("Title1");
		par1->SetText("NewTestText");
		img1->Resize(69u, 42u);

		document.Undo();
		document.Undo();
		document.Undo();
		document.Undo();
	}
};

BOOST_FIXTURE_TEST_SUITE(Document, DocumentFixture)
	BOOST_FIXTURE_TEST_CASE(returns_error_when_trying_to_save_file_which_is_already_exists, AlreadyCreatedHtmlFileFixture)
	{
		BOOST_CHECK_THROW(document.Save(createdFile), std::runtime_error);
	}

	BOOST_AUTO_TEST_CASE(returns_error_when_cant_open_file_with_not_created_folder)
	{
		BOOST_CHECK_THROW(document.Save("notCreatedFolder//test.html"), std::runtime_error);
	}

	BOOST_AUTO_TEST_CASE(returns_error_while_cant_create_folder_with_current_filename)
	{
		BOOST_CHECK_THROW(document.Save("*?.html"), std::runtime_error);
	}

	BOOST_AUTO_TEST_CASE(prints_empty_document)
	{
		document.Save(testFilePath);
		std::string result = GetResultString();
		BOOST_CHECK_EQUAL(result, R"text(<!DOCTYPE html><html><head><meta http-equiv="Content-Type" content="text/html; charset=windows-1251"/><title>Untitled document</title></head><body><h1>Untitled document</h1></body></html>)text");
	}

	BOOST_AUTO_TEST_CASE(prints_correct_title_with_html_encoded_text)
	{
		document.SetTitle("<a href=\"//test.com\"></a>&\\");
		document.Save(testFilePath);
		std::string result = GetResultString();
		BOOST_CHECK_EQUAL(result, R"text(<!DOCTYPE html><html><head><meta http-equiv="Content-Type" content="text/html; charset=windows-1251"/><title>&lt;a href=&quot;//test.com&quot;&gt;&lt;/a&gt;&amp;\</title></head><body><h1>&lt;a href=&quot;//test.com&quot;&gt;&lt;/a&gt;&amp;\</h1></body></html>)text");
	}

	BOOST_AUTO_TEST_CASE(wont_create_images_folder_without_images)
	{
		document.InsertParagraph("Test");
		document.Save(testFilePath);

		FileUtils::Delete(testFilePath);
		BOOST_CHECK(boost::filesystem::is_empty(testFileFolderPath));
	}

	BOOST_AUTO_TEST_CASE(prints_paragraph_with_html_encoded_text)
	{
		document.InsertParagraph("<a href=\"//test.com\"></a>&\\");
		document.Save(testFilePath);
		std::string result = GetResultString();
		BOOST_CHECK_EQUAL(result, R"text(<!DOCTYPE html><html><head><meta http-equiv="Content-Type" content="text/html; charset=windows-1251"/><title>Untitled document</title></head><body><h1>Untitled document</h1><p>&lt;a href=&quot;//test.com&quot;&gt;&lt;/a&gt;&amp;\</p></body></html>)text");
	}

	BOOST_AUTO_TEST_SUITE(while_prints_image)
		BOOST_AUTO_TEST_CASE(returns_correct_size)
		{
			auto image = document.InsertImage(testImage, 640u, 480u);
			document.Save(testFilePath);
			std::string result = GetResultString();

			auto imagePath = image->GetPath().filename();
			BOOST_CHECK_EQUAL(result, (boost::format(R"text(<!DOCTYPE html><html><head><meta http-equiv="Content-Type" content="text/html; charset=windows-1251"/><title>Untitled document</title></head><body><h1>Untitled document</h1><img src="test_images%1%" alt="" style="width:640px;height:480px;"></img></body></html>)text") % ("\\" + imagePath.string())).str());
		}

		BOOST_AUTO_TEST_CASE(returns_src_path_relative_to_html_file)
		{
			auto image = document.InsertImage(testImage, 640u, 480u);

			testFileFolderPath = testFileFolderPath / "some" / "path";
			testFilePath = testFileFolderPath / "test.html";
			FileUtils::CreateDirReqursively(testFileFolderPath);

			document.Save(testFilePath);
			std::string result = GetResultString();

			auto imagePath = image->GetPath().filename();
			BOOST_CHECK_EQUAL(result, (boost::format(R"text(<!DOCTYPE html><html><head><meta http-equiv="Content-Type" content="text/html; charset=windows-1251"/><title>Untitled document</title></head><body><h1>Untitled document</h1><img src="test_images%1%" alt="" style="width:640px;height:480px;"></img></body></html>)text") % ("\\" + imagePath.string())).str());
		}

		BOOST_AUTO_TEST_CASE(makes_images_folder_for_resources_beside_the_file)
		{
			auto image = document.InsertImage(testImage, 640u, 480u);
			document.Save(testFilePath);
			auto resourceFolderPath = testFilePath.parent_path() / (testFilePath.filename().replace_extension("").string() + "_images");
			BOOST_CHECK(FileUtils::Exists(resourceFolderPath));
		}

		BOOST_AUTO_TEST_CASE(makes_random_images_folder_if_default_is_already_exists)
		{
			auto defaultResourceFolderPath = testFilePath.parent_path() / (testFilePath.filename().replace_extension("").string() + "_images");
			FileUtils::CreateDirReqursively(defaultResourceFolderPath);

			auto image = document.InsertImage(testImage, 640u, 480u);
			document.Save(testFilePath);

			auto folders = GetFoldersByRegex(testFileFolderPath, boost::regex("test_images_.*"));
			BOOST_CHECK_EQUAL(folders.size(), 1);

			auto resourceFolder = folders[0];
			auto imageResourcePath = resourceFolder / image->GetPath().filename();
			BOOST_CHECK(FileUtils::Exists(imageResourcePath));

			auto imageResourceRelativePath = resourceFolder.filename() / image->GetPath().filename();
			auto result = GetResultString();
			BOOST_CHECK_EQUAL(result, (boost::format(R"text(<!DOCTYPE html><html><head><meta http-equiv="Content-Type" content="text/html; charset=windows-1251"/><title>Untitled document</title></head><body><h1>Untitled document</h1><img src="%1%" alt="" style="width:640px;height:480px;"></img></body></html>)text") % (imageResourceRelativePath.string())).str());
		}

		BOOST_AUTO_TEST_CASE(returns_error_while_failed_to_export_image)
		{
			auto image = document.InsertImage(testImage, 640u, 480u);
			FileUtils::Delete(image->GetPath());

			BOOST_CHECK_THROW(document.Save(testFilePath), std::runtime_error);
		}
	BOOST_AUTO_TEST_SUITE_END()

	BOOST_FIXTURE_TEST_CASE(prints_items_in_document_items_order, NotEmptyDocumentFixture)
	{
		document.Save(testFilePath);
		std::string result = GetResultString();
		BOOST_CHECK_EQUAL(result, (boost::format(R"text(<!DOCTYPE html><html><head><meta http-equiv="Content-Type" content="text/html; charset=windows-1251"/><title>Title</title></head><body><h1>Title</h1><p>Test</p><img src="test_images%1%" alt="" style="width:640px;height:480px;"></img><p>Test1</p></body></html>)text") % ("\\" + img1->GetPath().filename().string())).str());
	}
BOOST_AUTO_TEST_SUITE_END()