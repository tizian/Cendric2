#include "TextProvider.h"
#include "ResourceManager.h"
#include "DatabaseManager.h"
#include "GUI/BitmapText.h"

using namespace std;

TextProvider* g_textProvider;

const int TextProvider::TAB_TO_SPACES = 4;

TextProvider::TextProvider() {
	reload();
}

void TextProvider::reload() {
	setLanguage(g_resourceManager->getConfiguration().language);
}



std::string TextProvider::getText(const std::string& key) {
	return getText(key, "core");
}

std::string TextProvider::getText(const std::string& key, const std::string& type, bool isReplaceItemVariables, bool isRaw) {
	if (key.empty()) return "";

	std::string query = "SELECT " + m_language + " FROM text WHERE text_id = '" + key + "' AND text_type = '" + type + "' LIMIT 1;";
	ResultSet rs = g_databaseManager->query(query);
	if (rs.empty() || rs[0].empty()) {
		// fallback
		g_logger->logWarning("TranslationReader", "Tried to get missing translation for key: " + key + " with type " + type);
		return "(undefined text " + key + ")";
	}

	if (isReplaceItemVariables) {
		replaceItemVariables(rs[0][0]);
	}

	if (isRaw) {
		return rs[0][0];
	}
	else {
		return transform(rs[0][0]);
	}
}

void TextProvider::replaceItemVariables(std::string& text) {
	if (text.find('$') == std::string::npos) {
		// nothing to do
		return;
	}
	std::string remainingText = text;
	std::size_t varPos = remainingText.find('$');
	text.clear();
	while (varPos != std::string::npos) {
		text.append(remainingText.substr(0, varPos));
		remainingText = remainingText.substr(varPos + 1);
		varPos = remainingText.find('$'); 
		if (varPos == std::string::npos) {
			g_logger->logError("TextProvider", "missing closing $ of item id to parse.");
			break;
		}
		std::string itemId = remainingText.substr(0, varPos);
		text.append(getText(itemId, "item", false));
		remainingText = remainingText.substr(varPos + 1);
		varPos = remainingText.find('$');
	}
	text.append(remainingText);
}

std::string TextProvider::getCroppedText(const std::string& key, int characterSize, int maxWidth, bool isRaw) {
	return getCroppedText(key, "core", characterSize, maxWidth, isRaw);
}

std::string TextProvider::getCroppedText(const std::string& key, const std::string& type, int characterSize, int maxWidth, bool isRaw) {
	return getCroppedString(getText(key, type, true, isRaw), characterSize, maxWidth);
}

std::string TextProvider::getCroppedString(const std::string& string, int characterSize, int maxWidth) {
	// preconditions
	if (characterSize < 1 || maxWidth < characterSize) {
		return "";
	}

	size_t maxLineChars = static_cast<size_t>(maxWidth / characterSize);
	std::string uncroppedText = string;
	std::string text = "";
	while (static_cast<int>(uncroppedText.size()) * characterSize > maxWidth) {
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
		size_t space = maxLineChars;
		while (true) {
			found = uncroppedText.find(" ");
			if (found != string::npos && found++ < space) {
				text.append(uncroppedText.substr(0, found));
				uncroppedText = uncroppedText.substr(found);
				space = space - static_cast<int>(found);
			}
			else if (found == string::npos || space >= found) {
				text.append(uncroppedText.substr(0, space));
				text.append("\n");
				uncroppedText = uncroppedText.substr(space);
				break;
			}
			else {
				text.append("\n");
				break;
			}
		}
	}
	return text.append(uncroppedText);
}

std::string TextProvider::transform(const std::string& str) {
	std::string out;
	for (size_t i = 0; i < str.length(); ++i) {
		unsigned char c = str.at(i);
		if (c == 0xc3u) {
			unsigned char c2 = str.at(i + 1);
			out.push_back(c2 + 0x40u);
			++i;
		}
		else if (c == 0xc2u) {
			unsigned char c2 = str.at(i + 1);
			out.push_back(c2);
			++i;
		}
		else if (c == 0x09u) {
			// convert tabs to spaces
			for (int j = 0; j < TAB_TO_SPACES; ++j) {
				out.push_back(0x20u);
			}
		}
		else {
			out.push_back(c);
		}
	}
	return out;
}

void TextProvider::setLanguage(Language lang) {
	switch (lang) {
	case Language::Lang_DE:
		m_language = "german";
		break;
	case Language::Lang_CH:
		m_language = "swiss_german";
		break;
	case Language::LANG_ES:
		m_language = "spanish";
		break;
	case Language::Lang_EN:
	default:
		m_language = "english";
		break;
	}
}

