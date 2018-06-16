#pragma once
#include "stdafx.h"
#include "IDocumentExporter.h"
#include "Document.h"

class CDocumentHtmlExporter : public IDocumentExporter
{
public:
	CDocumentHtmlExporter(CDocument const& document, boost::filesystem::path const& pathToHtmlFile);
	~CDocumentHtmlExporter();

	void AddParagraph(std::shared_ptr<IParagraph> const& paragraph) override;
	void AddImage(std::shared_ptr<IImage> const& image) override;

private:
	void OpenFileForSave(std::string const& path);
	void WriteDocumentHeader(CDocument const& document);
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