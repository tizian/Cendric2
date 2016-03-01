#include "TextProvider.h"
#include "ResourceManager.h"
#include "DatabaseManager.h"

using namespace std;

TextProvider* g_textProvider;

void TextProvider::reload() {
	setLanguage(g_resourceManager->getConfiguration().language);
}

std::string TextProvider::getText(const std::string& key) {
	return getText(key, "core");
}

std::string TextProvider::getText(const std::string& key, const std::string& type) {
	std::string query = "SELECT " + m_language + " FROM text WHERE text_id = '" + key + "' AND text_type = '" + type + "' LIMIT 1;";
	ResultSet rs = g_databaseManager->query(query);
	if (rs.empty() || rs[0].empty()) {
		// fallback
		g_logger->logWarning("TranslationReader", "Tried to get missing translation for key: " + key + " with type " + type);
		return "(undefined text " + key + ")";
	}
	return rs[0][0];
}

std::string TextProvider::getCroppedText(const std::string& key, int characterSize, int maxWidth) {
	return getCroppedText(key, "core", characterSize, maxWidth);
}

std::string TextProvider::getCroppedText(const std::string& key, const std::string& type, int characterSize, int maxWidth) {
	return getCroppedString(getText(key, type), characterSize, maxWidth);
}

std::string TextProvider::getCroppedString(const std::string& string, int characterSize, int maxWidth) {
	// preconditions
	if (characterSize < 1 || maxWidth < characterSize) {
		return "";
	}

	int maxLineChars = maxWidth / characterSize;
	std::string uncroppedText = string;
	std::string text = "";
	while (uncroppedText.size() * characterSize > maxWidth) {
		// check for forced newlines
		size_t found = uncroppedText.find("\n");
		if (found != string::npos && found < maxLineChars && found++ != string::npos) {
			text.append(uncroppedText.substr(0, found));
			uncroppedText = uncroppedText.substr(found);
			continue;
		}
		// check if we need to crop a whole word
		found = uncroppedText.find(" ");
		if (found == string::npos || (found != string::npos && found > maxLineChars)) {
			text.append(uncroppedText.substr(0, maxLineChars));
			text.append("\n");
			uncroppedText = uncroppedText.substr(maxLineChars);
			continue;
		}
		// append words as long as we have still space for them
		int space = maxLineChars;
		while (true) {
			found = uncroppedText.find(" ");
			if (found != string::npos && found++ < space) {
				text.append(uncroppedText.substr(0, found));
				uncroppedText = uncroppedText.substr(found);
				space = space - static_cast<int>(found);
			}
			else {
				text.append("\n");
				break;
			}
		}
	}
	return text.append(uncroppedText);
}

void TextProvider::setLanguage(Language lang) {
	switch (lang) {
	case Language::Lang_DE:
		m_language = "german";
		break;
	case Language::Lang_CH:
		m_language = "swiss_german";
		break;
	case Language::Lang_EN:
	default:
		m_language = "english";
		break;
	}
}

