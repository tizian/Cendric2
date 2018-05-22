#pragma once

#include "TextProvider.h"
#include "Enums/EnumNames.h"

inline std::string getHintTitle(const std::string& hintKey) {
	return g_textProvider->getText(hintKey, "hint");
}

inline std::string getKeyName(Key key) {
	auto& mainMap = g_resourceManager->getConfiguration().mainKeyMap;
	if (!g_inputController->isJoystickConnected() && contains(mainMap, key)) {
		return EnumNames::getKeyboardKeyName(mainMap.at(key));
	}

	if (key == Key::Up) {
		return g_textProvider->getText("Up");
	}
	if (key == Key::Down) {
		return g_textProvider->getText("Down");
	}
	if (key == Key::Left) {
		return g_textProvider->getText("Left");
	}
	if (key == Key::Right) {
		return g_textProvider->getText("Right");
	}

	auto& keymap = g_resourceManager->getConfiguration().joystickKeyMap;

	if (key == Key::Inventory ||
		key == Key::CharacterInfo ||
		key == Key::Journal ||
		key == Key::Map ||
		key == Key::Spellbook) {
		return EnumNames::getGamepadAxisName(keymap.at(Key::Menu));
	}

	if (contains(keymap, key)) {
		return EnumNames::getGamepadAxisName(keymap.at(key));
	}
	
	if (contains(mainMap, key)) {
		return EnumNames::getKeyboardKeyName(mainMap.at(key));
	}

	return std::string("?");
}

inline std::string getHintDescription(const std::string& hintKey) {
	std::string hintText = (g_textProvider->getText("Press", "hint_desc")) + " ";

	if (hintKey == "Inventory") {
		hintText.append(getKeyName(Key::Inventory) + " ");
	}
	else if (hintKey == "Chop") {
		hintText.append(getKeyName(Key::Chop) + " ");
	}
	else if (hintKey == "Jump") {
		hintText.append(getKeyName(Key::Jump) + " ");
	}
	else if (hintKey == "Journal") {
		hintText.append(getKeyName(Key::Journal) + " ");
	}
	else if (hintKey == "Spellbook") {
		hintText.append(getKeyName(Key::Spellbook) + " ");
	}
	else if (hintKey == "CharacterInfo") {
		hintText.append(getKeyName(Key::CharacterInfo) + " ");
	}
	else if (hintKey == "Map" || hintKey == "Waypoint") {
		hintText.append(getKeyName(Key::Map) + " ");
	}
	else if (hintKey == "Pickup") {
		hintText.append(getKeyName(Key::Interact) + " ");
	}
	else if (hintKey == "Reload") {
		hintText.append(getKeyName(Key::BackToCheckpoint) + " ");
	}
	else if (hintKey == "LeaveLevel") {
		hintText.append(getKeyName(Key::Up) + " ");
	}
	else if (hintKey == "Scout") {
		hintText.append(getKeyName(Key::Up) + " ");
		hintText.append(g_textProvider->getText("And") + " ");
		hintText.append(getKeyName(Key::Down) + " ");
	}
	else if (hintKey == "Highlight") {
		hintText.append(getKeyName(Key::ToggleTooltips) + " ");
	}
	else if (hintKey == "MapMove") {
		hintText.append(getKeyName(Key::Up) + ", ");
		hintText.append(getKeyName(Key::Left) + ", ");
		hintText.append(getKeyName(Key::Down) + " ");
		hintText.append(g_textProvider->getText("And") + " ");
		hintText.append(getKeyName(Key::Right) + " ");
	}
	else {
		hintText.clear();
	}
	hintText.append(g_textProvider->getText(hintKey, "hint_desc"));

	return hintText;
}