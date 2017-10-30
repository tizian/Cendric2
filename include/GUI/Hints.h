#pragma once

#include "TextProvider.h"
#include "Enums/EnumNames.h"

inline std::string getHintTitle(const std::string& hintKey) {
	return g_textProvider->getText(hintKey, "hint");
}

inline std::string getHintDescription(const std::string& hintKey) {
	std::string hintText = (g_textProvider->getText("Press", "hint_desc")) + " ";

	if (hintKey.compare("Inventory") == 0) {
		hintText.append(EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap.at(Key::Inventory)) + " ");
	}
	else if (hintKey.compare("Chop") == 0) {
		hintText.append(EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap.at(Key::Chop)) + " ");
	}
	else if (hintKey.compare("Jump") == 0) {
		hintText.append(EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap.at(Key::Jump)) + " ");
	}
	else if (hintKey.compare("Journal") == 0) {
		hintText.append(EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap.at(Key::Journal)) + " ");
	}
	else if (hintKey.compare("Spellbook") == 0) {
		hintText.append(EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap.at(Key::Spellbook)) + " ");
	}
	else if (hintKey.compare("CharacterInfo") == 0) {
		hintText.append(EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap.at(Key::CharacterInfo)) + " ");
	}
	else if (hintKey.compare("Map") == 0 || hintKey.compare("Waypoint") == 0) {
		hintText.append(EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap.at(Key::Map)) + " ");
	}
	else if (hintKey.compare("Pickup") == 0) {
		hintText.append(EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap.at(Key::Interact)) + " ");
	}
	else if (hintKey.compare("Reload") == 0) {
		hintText.append(EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap.at(Key::BackToCheckpoint)) + " ");
	}
	else if (hintKey.compare("LeaveLevel") == 0) {
		hintText.append(EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap.at(Key::Up)) + " ");
	}
	else if (hintKey.compare("Scout") == 0) {
		hintText.append(EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap.at(Key::Up)) + " ");
		hintText.append(g_textProvider->getText("And") + " ");
		hintText.append(EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap.at(Key::Down)) + " ");
	}
	else if (hintKey.compare("Highlight") == 0) {
		hintText.append(EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap.at(Key::ToggleTooltips)) + " ");
	}
	else if (hintKey.compare("MapMove") == 0) {
		hintText.append(EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap.at(Key::Up)) + ", ");
		hintText.append(EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap.at(Key::Left)) + ", ");
		hintText.append(EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap.at(Key::Down)) + " ");
		hintText.append(g_textProvider->getText("And") + " ");
		hintText.append(EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap.at(Key::Right)) + " ");
	}
	else {
		hintText.clear();
	}
	hintText.append(g_textProvider->getText(hintKey, "hint_desc"));

	return hintText;
}