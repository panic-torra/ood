#include "stdafx.h"
#include "../command/EditCommand.h"
#include "../DocumentEditor/Document.h"
#include "../DocumentEditor/FileUtils.h"

struct EditorFixture
{
	std::stringstream input;
	std::stringstream output;

	CDocument document;
	CEditCommand editor;

	EditorFixture()
		: editor(document, input, output)
	{
		ResetInputOutput();
	}

	void VerifyCommandHandling(std::string const& cmd, std::string const& expectedOutput)
	{
		ResetInputOutput();
		BOOST_CHECK(input << cmd);
		BOOST_CHECK_NO_THROW(editor.HandleCommand());
		BOOST_CHECK(input.eof());
		auto result = output.str();

		BOOST_CHECK_EQUAL(output.str(), expectedOutput);
	}

	void ResetInputOutput()
	{
		output = std::stringstream();
		input = std::stringstream();
	}
};

BOOST_FIXTURE_TEST_SUITE(Editor, EditorFixture)

BOOST_AUTO_TEST_CASE(returns_command_list_if_handled_unknown_command)
{
	VerifyCommandHandling("UnknownCommand", R"text(Unknown command!

InsertParagraph <position=end> [text]
InsertImage <position=end> <image> <width> <height>
SetTitle <title>
ReplaceText <index> [text]
ResizeImage <index> <width> <height>
DeleteItem <index>
Undo
Redo
Save <path>

Title: Untitled document
Empty!
History: ----/----


> )text");
}

BOOST_AUTO_TEST_CASE(returns_error_when_insertParagraph_command_executed_without_correct_position_params)
{
	VerifyCommandHandling("InsertParagraph Some paragraph text", R"text(Error!
Position must be an unsigned integer or "end"!
InsertParagraph <position=end> [text]

Title: Untitled document
Empty!
History: ----/----


> )text");

	VerifyCommandHandling("InsertParagraph", R"text(Error!
Position must be an unsigned integer or "end"!
InsertParagraph <position=end> [text]

Title: Untitled document
Empty!
History: ----/----


> )text");
}

BOOST_AUTO_TEST_CASE(can_handle_insertParagraph_command)
{
	VerifyCommandHandling("InsertParagraph end Some paragraph text", R"text(Success!


Title: Untitled document
0: Paragraph: Some paragraph text
History: Undo/----


> )text");

	VerifyCommandHandling("InsertParagraph 0 Some new paragraph text", R"text(Success!


Title: Untitled document
0: Paragraph: Some new paragraph text
1: Paragraph: Some paragraph text
History: Undo/----


> )text");
}

BOOST_AUTO_TEST_CASE(returns_error_where_InsertImage_command_executed_without_correct_params)
{
	VerifyCommandHandling("InsertImage", R"text(Error!
Position must be an unsigned integer or "end"!
InsertImage <position=end> <imagePath> <width> <height>

Title: Untitled document
Empty!
History: ----/----


> )text");

	VerifyCommandHandling("InsertImage end", R"text(Error!
ImagePath is required!
InsertImage <position=end> <imagePath> <width> <height>

Title: Untitled document
Empty!
History: ----/----


> )text");

	VerifyCommandHandling("InsertImage end test.jpg", R"text(Error!
Width must be an unsigned integer!
InsertImage <position=end> <imagePath> <width> <height>

Title: Untitled document
Empty!
History: ----/----


> )text");

	VerifyCommandHandling("InsertImage end test.jpg a", R"text(Error!
Width must be an unsigned integer!
InsertImage <position=end> <imagePath> <width> <height>

Title: Untitled document
Empty!
History: ----/----


> )text");

	VerifyCommandHandling("InsertImage end test.jpg 640", R"text(Error!
Height must be an unsigned integer!
InsertImage <position=end> <imagePath> <width> <height>

Title: Untitled document
Empty!
History: ----/----


> )text");

	VerifyCommandHandling("InsertImage end test.jpg 640 b", R"text(Error!
Height must be an unsigned integer!
InsertImage <position=end> <imagePath> <width> <height>

Title: Untitled document
Empty!
History: ----/----


> )text");
}

BOOST_AUTO_TEST_CASE(can_handle_insertImage_command)
{
	BOOST_CHECK(input << "InsertImage end test.jpg 640 480");
	editor.HandleCommand();
	BOOST_CHECK_EQUAL(document.GetItemsCount(), 1);
	auto imagePath = document.GetItem(0)->GetImage()->GetPath();

	BOOST_CHECK_EQUAL(output.str(), (boost::format(R"text(Success!


Title: Untitled document
0: Image: 640 480 "%1%"
History: Undo/----


> )text") % imagePath.string()).str());
}

BOOST_AUTO_TEST_CASE(can_handle_insertImage_command_by_position)
{
	BOOST_CHECK(input << "InsertImage 0 test.jpg 640 480");
	editor.HandleCommand();
	BOOST_CHECK_EQUAL(document.GetItemsCount(), 1);
	auto imagePath = document.GetItem(0)->GetImage()->GetPath();

	BOOST_CHECK_EQUAL(output.str(), (boost::format(R"text(Success!


Title: Untitled document
0: Image: 640 480 "%1%"
History: Undo/----


> )text") % imagePath.string()).str());
}

BOOST_AUTO_TEST_CASE(can_handle_setTitle_command)
{
	VerifyCommandHandling("SetTitle New document title", R"text(Success!


Title: New document title
Empty!
History: Undo/----


> )text");
}

BOOST_AUTO_TEST_CASE(returns_error_where_ReplaceText_command_executed_without_correct_params)
{
	auto image = document.InsertImage("test.jpg", 640, 480);

	VerifyCommandHandling("ReplaceText ", (boost::format(R"text(Error!
Index is required!

Title: Untitled document
0: Image: 640 480 "%1%"
History: Undo/----


> )text") % image->GetPath().string()).str());

	VerifyCommandHandling("ReplaceText 0", (boost::format(R"text(Error!
The specified item is not a paragraph!

Title: Untitled document
0: Image: 640 480 "%1%"
History: Undo/----


> )text") % image->GetPath().string()).str());
}

BOOST_AUTO_TEST_CASE(can_handle_ReplaceText_command)
{
	document.InsertParagraph("Old text");
	VerifyCommandHandling("ReplaceText 0 Some new text", R"text(Success!


Title: Untitled document
0: Paragraph: Some new text
History: Undo/----


> )text");
}

BOOST_AUTO_TEST_CASE(returns_error_where_ResizeImage_command_executed_without_correct_params)
{
	document.InsertParagraph("TestParagraph");

	VerifyCommandHandling("ResizeImage", R"text(Error!
Index is required!
ResizeImage <index> <width> <height>

Title: Untitled document
0: Paragraph: TestParagraph
History: Undo/----


> )text");

	VerifyCommandHandling("ResizeImage 0", R"text(Error!
Width must be an unsigned integer!
ResizeImage <index> <width> <height>

Title: Untitled document
0: Paragraph: TestParagraph
History: Undo/----


> )text");

	VerifyCommandHandling("ResizeImage 0 640", R"text(Error!
Height must be an unsigned integer!
ResizeImage <index> <width> <height>

Title: Untitled document
0: Paragraph: TestParagraph
History: Undo/----


> )text");

	VerifyCommandHandling("ResizeImage 0 640 480", R"text(Error!
The specified item is not a paragraph!

Title: Untitled document
0: Paragraph: TestParagraph
History: Undo/----


> )text");
}

BOOST_AUTO_TEST_CASE(can_handle_ResizeImage_command)
{
	auto image = document.InsertImage("test.jpg", 640, 480);

	VerifyCommandHandling("ResizeImage 0 1024 768", (boost::format(R"text(Success!


Title: Untitled document
0: Image: 1024 768 "%1%"
History: Undo/----


> )text") % image->GetPath().string()).str());
}

BOOST_AUTO_TEST_CASE(can_handle_DeleteItem_command)
{
	document.InsertParagraph("TestParagraph");

	VerifyCommandHandling("DeleteItem", R"text(Error!
Index is required!

Title: Untitled document
0: Paragraph: TestParagraph
History: Undo/----


> )text");

	VerifyCommandHandling("DeleteItem 0", R"text(Success!


Title: Untitled document
Empty!
History: Undo/----


> )text");
}

BOOST_AUTO_TEST_CASE(can_handle_Undo_command)
{
	VerifyCommandHandling("Undo", R"text(Error!
Nothing to undo!

Title: Untitled document
Empty!
History: ----/----


> )text");

	document.InsertParagraph("TestParagraph");

	VerifyCommandHandling("Undo", R"text(Success!


Title: Untitled document
Empty!
History: ----/Redo


> )text");
}

BOOST_AUTO_TEST_CASE(can_handle_Redo_command)
{
	VerifyCommandHandling("Redo", R"text(Error!
Nothing to redo!

Title: Untitled document
Empty!
History: ----/----


> )text");

	document.InsertParagraph("TestParagraph");
	document.Undo();

	VerifyCommandHandling("Redo", R"text(Success!


Title: Untitled document
0: Paragraph: TestParagraph
History: Undo/----


> )text");
}

BOOST_AUTO_TEST_CASE(can_handle_Save_command)
{
	VerifyCommandHandling("Save", R"text(Error!
Save path is required!

Title: Untitled document
Empty!
History: ----/----


> )text");

	std::string testFile = "test.html";

	ResetInputOutput();
	BOOST_CHECK(input << "Save " + testFile);
	BOOST_CHECK_NO_THROW(editor.HandleCommand());
	BOOST_CHECK(FileUtils::Exists(testFile));
	FileUtils::Delete(testFile);
}

BOOST_AUTO_TEST_SUITE_END()