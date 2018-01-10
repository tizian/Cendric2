#pragma once

#include "TextProvider.h"
#include "Enums/EnumNames.h"

inline std::string getHintTitle(const std::string& hintKey) {
	return g_textProvider->getText(hintKey, "hint");
}

inline std::string getHintDescription(const std::string& hintKey) {
	std::string hintText = (g_textProvider->getText("Press", "hint_desc")) + " ";

	if (hintKey == "Inventory") {
		hintText.append(EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap.at(Key::Inventory)) + " ");
	}
	else if (hintKey == "Chop") {
		hintText.append(EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap.at(Key::Chop)) + " ");
	}
	else if (hintKey == "Jump") {
		hintText.append(EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap.at(Key::Jump)) + " ");
	}
	else if (hintKey == "Journal") {
		hintText.append(EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap.at(Key::Journal)) + " ");
	}
	else if (hintKey == "Spellbook") {
		hintText.append(EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap.at(Key::Spellbook)) + " ");
	}
	else if (hintKey == "CharacterInfo") {
		hintText.append(EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap.at(Key::CharacterInfo)) + " ");
	}
	else if (hintKey == "Map" || hintKey == "Waypoint") {
		hintText.append(EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap.at(Key::Map)) + " ");
	}
	else if (hintKey == "Pickup") {
		hintText.append(EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap.at(Key::Interact)) + " ");
	}
	else if (hintKey == "Reload") {
		hintText.append(EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap.at(Key::BackToCheckpoint)) + " ");
	}
	else if (hintKey == "LeaveLevel") {
		hintText.append(EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap.at(Key::Up)) + " ");
	}
	else if (hintKey == "Scout") {
		hintText.append(EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap.at(Key::Up)) + " ");
		hintText.append(g_textProvider->getText("And") + " ");
		hintText.append(EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap.at(Key::Down)) + " ");
	}
	else if (hintKey == "Highlight") {
		hintText.append(EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap.at(Key::ToggleTooltips)) + " ");
	}
	else if (hintKey == "MapMove") {
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