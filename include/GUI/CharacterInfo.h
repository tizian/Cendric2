#pragma once

#include "global.h"
#include "Level/LevelMainCharacter.h"
#include "Window.h"
#include "GUI/GUIConstants.h"

// the character info, as displayed in a level
// it takes its information from an attribute bean (of the level main char or the character core)
class CharacterInfo {
public:
	CharacterInfo(const AttributeData* attributes);
	~CharacterInfo();

	void show();
	void hide();

	void update();
	void render(sf::RenderTarget& target) const;
	bool isVisible() const;

	// can be called from outside to trigger a reload as soon as the window is visible
	void notifyChange();

private:
	// reloads the character info text, depending on the main char
	void reload();
	const AttributeData* m_attributes;
	bool m_isVisible = false;
	bool m_isReloadNeeded = true;

	Window* m_window = nullptr;
	BitmapText m_title;
	BitmapText m_namesText;
	BitmapText m_attributeText;
};