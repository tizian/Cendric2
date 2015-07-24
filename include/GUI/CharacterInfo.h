#pragma once

#include "global.h"
#include "LevelMainCharacter.h"
#include "Window.h"

// the character info, as displayed in a level
// it takes its information from the level main character
class CharacterInfo
{
public:
	CharacterInfo(LevelMainCharacter* mainChar);
	~CharacterInfo();

	void show();
	void hide();

	void render(sf::RenderTarget& target) const;

private:
	LevelMainCharacter* m_mainChar;
	const AttributeBean* m_attributes;
	bool m_isVisible = false;

	// reloads the character info text, depending on the main char
	void reload();

	Window* m_window;
	BitmapText m_characterInfoText;

	const float TEXT_OFFSET = 20.f;
	const float CHARACTERINFO_TOP = 50.f;
	const float CHARACTERINFO_LEFT = 50.f;
	const float CHARACTERINFO_HEIGHT = 500.f;
	const float CHARACTERINFO_WIDTH = 700.f;
};