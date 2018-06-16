#include "stdafx.h"
#include "DocumentExporter.h"
#include "FileUtils.h"

CDocumentHtmlExporter::CDocumentHtmlExporter(CDocument const& document, boost::filesystem::path const& pathToHtmlFile)
	: m_resultFilePath(pathToHtmlFile)
{
	if (FileUtils::Exists(pathToHtmlFile))
	{
		throw std::runtime_error("Current file is already exists!");
	}

	OpenFileForSave(pathToHtmlFile.string());
	WriteDocumentHeader(document);
}

CDocumentHtmlExporter::~CDocumentHtmlExporter()
{
	try
	{
		WriteDocumentFooter();
	}
	catch (...)
	{
	}
}

void CDocumentHtmlExporter::AddParagraph(std::shared_ptr<IParagraph> const& paragraph)
{
	m_resultFile << "<p>" << EncodeString(paragraph->GetText()) << "</p>";
}

void CDocumentHtmlExporter::AddImage(std::shared_ptr<IImage> const& image)
{
	boost::filesystem::path relativeResourcePath = CopyImageResource(image->GetPath());
	m_resultFile << (boost::format(R"text(<img src="%1%" alt="" style="width:%2%px;height:%3%px;"></img>)text") % relativeResourcePath.string() % image->GetWidth() % image->GetHeight());
}

void CDocumentHtmlExporter::OpenFileForSave(std::string const& path)
{
	m_resultFile = std::ofstream(path, std::ios::app);
	if (!m_resultFile.is_open())
	{
		throw std::runtime_error("Failed to open file for export!");
	}
}

void CDocumentHtmlExporter::WriteDocumentHeader(CDocument const& document)
{
	m_resultFile << (boost::format(R"text(<!DOCTYPE html><html><head><meta http-equiv="Content-Type" content="text/html; charset=windows-1251"/><title>%1%</title></head><body><h1>%1%</h1>)text") % EncodeString(document.GetTitle()));
}

void CDocumentHtmlExporter::WriteDocumentFooter()
{
	if (m_resultFile.good())
	{
		m_resultFile << "</body></html>";
	}
}

std::string CDocumentHtmlExporter::EncodeString(std::string const& str)
{
	std::string result(str);
	std::string::size_type pos = 0;
	for (;;)
	{
		pos = result.find_first_of("\"&<>", pos);
		if (pos == std::string::npos) break;
		std::string replacement;
		switch (result[pos])
		{
		case '\"': replacement = "&quot;"; break;
		case '&': replacement = "&amp;"; break;
		case '<': replacement = "&lt;"; break;
		case '>': replacement = "&gt;"; break;
		default:;
		}
		result.replace(pos, 1, replacement);
		pos += replacement.size();
	};
	return result;
}

boost::filesystem::path CDocumentHtmlExporter::CopyImageResource(boost::filesystem::path const& source)
{
	if (m_resultFileResourcesPath == boost::none)
	{
		m_resultFileResourcesPath = CreateResultFileResourcesPath(m_resultFilePath);
	}

	auto resourceFilename = source.filename();
	try
	{
		FileUtils::CopyFile(source, (*m_resultFileResourcesPath) / resourceFilename);
	}
	catch (std::runtime_error const&)
	{
		throw std::runtime_error("Failed to export image!");
	}

	return (*m_resultFileResourcesPath).filename() / resourceFilename;
}

boost::filesystem::path CDocumentHtmlExporter::CreateResultFileResourcesPath(boost::filesystem::path const& filePath)
{
	std::string resourcesFolderName = filePath.filename().replace_extension("").string() + "_images";
	boost::filesystem::path resourcesFolderPath = filePath.parent_path() / resourcesFolderName;

	boost::filesystem::path currentResourcesFolderPath = resourcesFolderPath;
	while (FileUtils::Exists(currentResourcesFolderPath))
	{
		currentResourcesFolderPath = boost::filesystem::unique_path(resourcesFolderPath.string() + "_%%%%%%");
	}

	FileUtils::CreateDirReqursively(currentResourcesFolderPath);
	return currentResourcesFolderPath;
}