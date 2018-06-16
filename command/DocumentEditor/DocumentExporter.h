#pragma once
#include "stdafx.h"
#include "Document.h"

class CDocumentHtmlExporter
{
public:
	CDocumentHtmlExporter(std::string const& documentTitle, boost::filesystem::path const& pathToHtmlFile);
	~CDocumentHtmlExporter();

	void AddParagraph(std::shared_ptr<IParagraph> const& paragraph);
	void AddImage(std::shared_ptr<IImage> const& image);

private:
	void OpenFileForSave(std::string const& path);
	void WriteDocumentHeader(std::string const& documentTitle);
	void WriteDocumentFooter();
	boost::filesystem::path CopyImageResource(boost::filesystem::path const& source);
	static boost::filesystem::path CreateResultFileResourcesPath(boost::filesystem::path const& filePath);
	static std::string EncodeString(std::string const& str);

	boost::filesystem::path m_resultFilePath;
	boost::optional<boost::filesystem::path> m_resultFileResourcesPath;

	std::vector<boost::filesystem::path> m_resources;
	std::ofstream m_resultFile;
	std::stringstream m_htmlResult;
};