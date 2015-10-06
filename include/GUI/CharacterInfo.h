#pragma once

#include "global.h"
#include "LevelMainCharacter.h"
#include "Window.h"
#include "GUI/GUIConstants.h"

// the character info, as displayed in a level
// it takes its information from an attribute bean (of the level main char or the character core)
class CharacterInfo {
public:
	CharacterInfo(const AttributeBean* attributes);
	~CharacterInfo();

	void show();
	void hide();

	void render(sf::RenderTarget& target) const;
	bool isVisible() const;

private:
	const AttributeBean* m_attributes;
	bool m_isVisible = false;

	// reloads the character info text, depending on the main char
	void reload();

	Window* m_window = nullptr;
	BitmapText m_title;
	BitmapText m_namesText;
	BitmapText m_attributeText;
};