#pragma once

#include "global.h"
#include "LevelMainCharacter.h"
#include "Window.h"

// the character info, as displayed in a level
// it takes its information from an attribute bean (of the level main char or the character core)
class CharacterInfo
{
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
	BitmapText m_characterInfoText;

	const float TEXT_OFFSET = 20.f;
	const float CHARACTERINFO_TOP = 100.f;
	const float CHARACTERINFO_LEFT = 50.f;
	const float CHARACTERINFO_HEIGHT = 500.f;
	const float CHARACTERINFO_WIDTH = 700.f;
};