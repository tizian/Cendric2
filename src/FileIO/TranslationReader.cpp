#include "FileIO/TranslationReader.h"
#include "ResourceManager.h"

using namespace std;

TranslationReader::TranslationReader()
{
}

TranslationReader::~TranslationReader()
{
}

bool TranslationReader::readTranslations(Language lang, std::map<std::string, std::wstring>& translationMap) const
{
	translationMap.clear();
	wstring contents = getFileContentsWide(g_resourceManager->getFilename(ResourceID::Translations));
	if (contents.empty())
	{
		return false;
	}
	StringTable tab;
	parseCsv((wchar_t *)contents.c_str(), tab);
	if (tab.size() == 0 || tab[0].size() < static_cast<int>(lang) + 1)
	{
		g_logger->logError("TranslationReader", "Error in translation file, incorrect number of columns or no rows");
		return false;
	}
	int lineNr = 0;
	for (auto it : tab)
	{
		lineNr++;
		string key = string(it[0].begin(), it[0].end());
		wstring value = it[static_cast<int>(lang)];
		if (key.empty() || value.empty())
		{
			g_logger->logError("TranslationReader", "Could not get translation for line: " + std::to_string(lineNr));
			return false;
		}
		if (translationMap.find(key) != translationMap.end())
		{
			g_logger->logWarning("TranslationReader", "Key: " + key + " has a duplicate translation");
			continue;
		}
		translationMap.insert({ key, value });
	}
	return true;
}